import subprocess
import csv
from io import StringIO
import numpy as np
import os
import pygame
import math
import threading
from queue import Queue
import time
import glob
from enum import Enum

class RadarHandler:
    def __init__(self, screen_size=(1200, 600), mode='live', file_path=None):
        pygame.init()
        self.screen_size = screen_size
        self.screen = pygame.display.set_mode(screen_size)
        
        # 모드에 따른 캡션 설정
        mode_text = {
            'live': 'LiveStream',
            'file': 'fileStream',
            'directory': 'directoryStream'
        }.get(mode, '알 수 없는 모드')
        pygame.display.set_caption(f"Radar Display - Original vs Filtered ({mode_text})")
        
        self.center_original = (screen_size[0]//4, screen_size[1]//2)  # 왼쪽 레이더 중심
        self.center_filtered = (3*screen_size[0]//4, screen_size[1]//2)  # 오른쪽 레이더 중심
        self.scale_factor = 250
        self.current_end_range = 50.0
        self.scale = 0
        self.concentric_circles = 5  # 동심원 개수
        self.data_queue = Queue()
        self.running = True
        self.angle_idx = 0
        self.current_sector = 0
        
        # 두 개의 서피스 초기화
        self.data_surface_original = pygame.Surface(screen_size)
        self.data_surface_original.fill((0, 0, 0))
        self.data_surface_filtered = pygame.Surface(screen_size)
        self.data_surface_filtered.fill((0, 0, 0))
        
        # 섹터 관련 변수
        self.sector_timestamps = {i: 0 for i in range(12)}
        
        # 프로세스 및 스레드 초기화
        self.process = None
        self.receiver_thread = None
        
        # 모드 설정 추가
        self.mode = mode
        self.file_path = file_path
        
        # directory 모드를 위한 변수 추가
        self.current_file_index = 0
        self.total_files = 0
        self.files = []
        self.scroll_rect = pygame.Rect(50, self.screen_size[1] - 30, self.screen_size[0] - 100, 20)
        self.scroll_button_rect = pygame.Rect(50, self.screen_size[1] - 30, 20, 20)
        self.dragging = False
        # 재생/정지 상태를 위한 변수 추가
        self.is_paused = False

    def draw_radar_display(self):
        # 왼쪽 레이더 (원본)
        self.draw_single_radar(self.center_original, "Original")
        # 오른쪽 레이더 (필터링됨)
        self.draw_single_radar(self.center_filtered, "Filtered")

    def draw_single_radar(self, center, title):
        # 레이더 배경 원 그리기
        pygame.draw.circle(self.screen, (0, 100, 0), center, self.scale_factor, 1)
        
        # 동심원과 거리 텍스트 그리기
        font = pygame.font.Font(None, 18)
        distance = np.linspace(0, self.current_end_range, self.concentric_circles + 1)
        # radius_ = np.linspace(0,self.scale_factor,6)
        for i in range(1, ((self.concentric_circles+1)-self.scale)):
            radius = self.scale_factor * i / (self.concentric_circles-self.scale)
            pygame.draw.circle(self.screen, (0, 50, 0), center, int(radius), 1)
            
            # distance = self.current_end_range * i / (5-self.scale)
            text = f"{distance[i]:.1f}m"
            text_surface = font.render(text, True, (0, 100, 0))
            text_rect = text_surface.get_rect()
            text_rect.midleft = (center[0] + int(radius) + 2, center[1]+10)
            self.screen.blit(text_surface, text_rect)
        
        # 방위각 선 그리기
        for angle in range(0, 360, 30):
            rad = math.radians(angle) - math.pi/2
            end_x = center[0] + self.scale_factor * math.cos(rad)
            end_y = center[1] + self.scale_factor * math.sin(rad)
            pygame.draw.line(self.screen, (0, 50, 0), center, (int(end_x), int(end_y)), 1)
        
        # 제목 표시
        title_font = pygame.font.Font(None, 36)
        title_surface = title_font.render(title, True, (0, 150, 0))
        title_rect = title_surface.get_rect()
        title_rect.midtop = (center[0], 10)
        self.screen.blit(title_surface, title_rect)

    def data_receiver(self):
        # received_count = 0
        last_stats_time = time.time()
        buffer = [[] for _ in range(12)]
        current_sector = 0
        
        try:
            while self.running:
                line = self.process.stdout.readline()
                if line:
                    try:
                        reader = csv.reader(StringIO(line.strip()))
                        for row in reader:
                            if len(row) >= 3:
                                azimuth = float(row[0])
                                sector_idx = int(azimuth // 30)
                                
                                if sector_idx != current_sector and buffer[current_sector]:
                                    if self.data_queue.qsize() < 10:
                                        self.data_queue.put((buffer[current_sector].copy(), time.time()))
                                        buffer[current_sector] = []
                                    else:
                                        print("큐가 가득 찼습니다. 데이터 스킵")
                                        buffer[current_sector] = []
                                
                                current_sector = sector_idx
                                buffer[sector_idx].append(row)
                                # received_count += 1
                    except Exception as e:
                        print(f"데이터 파싱 오류: {e}")
        except Exception as e:
            print(f"데이터 수신 오류: {e}")

    def data_receiver_directory(self):
        """디렉토리에서 레이더 데이터를 읽어오는 함수"""
        buffer = [[] for _ in range(12)]
        current_sector = 0
        last_file_index = -1  # 마지막으로 처리한 파일 인덱스 추적
        
        try:
            folder = self.file_path
            pattern = "*.txt"
            search_pattern = f"{folder}/{pattern}"
            self.files = sorted(glob.glob(search_pattern))
            self.total_files = len(self.files)
            
            if not self.files:
                print(f"경고: {search_pattern} 경로에서 파일을 찾을 수 없습니다.")
                return
            
            while self.running:
                # 파일 인덱스가 변경되었고 일시정지 상태일 때
                if self.is_paused and last_file_index != self.current_file_index:
                    # 버퍼 초기화
                    buffer = [[] for _ in range(12)]
                    current_sector = 0
                    
                    # 현재 선택된 파일의 데이터 즉시 처리
                    file = self.files[self.current_file_index]
                    with open(file, 'r') as f:
                        for line in f:
                            values = line.strip().split()
                            if not values:
                                continue
                                
                            try:
                                azimuth = float(values[0])
                                range_val = float(values[1])
                                timestamp = int(values[2])
                                intensity_data = values[3:]
                                
                                sector_idx = int(azimuth // 30)
                                
                                if sector_idx != current_sector and buffer[current_sector]:
                                    self.data_queue.put((buffer[current_sector].copy(), time.time()))
                                    buffer[current_sector] = []
                                
                                current_sector = sector_idx
                                buffer[sector_idx].append([str(azimuth), str(range_val), str(timestamp)] + intensity_data)
                                
                            except Exception as e:
                                print(f"데이터 파싱 오류: {e}")
                    
                    # 마지막 버퍼 데이터 처리
                    if buffer[current_sector]:
                        self.data_queue.put((buffer[current_sector].copy(), time.time()))
                    
                    last_file_index = self.current_file_index
                
                # 일시정지 상태일 때는 대기
                if self.is_paused:
                    time.sleep(0.1)
                    continue
                    
                # 기존 실시간 재생 로직
                file = self.files[self.current_file_index]
                with open(file, 'r') as f:
                    for line in f:
                        if not self.running:
                            return
                        
                        values = line.strip().split()
                        if not values:
                            continue
                            
                        try:
                            azimuth = float(values[0])
                            range_val = float(values[1])
                            timestamp = int(values[2])
                            intensity_data = values[3:]
                            
                            sector_idx = int(azimuth // 30)
                            
                            if sector_idx != current_sector and buffer[current_sector]:
                                if self.data_queue.qsize() < 20:
                                    self.data_queue.put((buffer[current_sector].copy(), time.time()))
                                    buffer[current_sector] = []
                                else:
                                    print("큐가 가득 찼습니다. 데이터 스킵")
                                    buffer[current_sector] = []
                            
                            current_sector = sector_idx
                            buffer[sector_idx].append([str(azimuth), str(range_val), str(timestamp)] + intensity_data)
                            
                        except Exception as e:
                            print(f"데이터 파싱 오류: {e}")
                    
                        time.sleep(0.0002)  # 데이터 처리 속도 조절
                
                last_file_index = self.current_file_index
                self.current_file_index += 1
                
                # 파일의 끝에 도달하면 처음으로 돌아가기
                if self.current_file_index >= self.total_files:
                    self.current_file_index = 0
                    # 데이터 서피스 초기화
                    self.data_surface_original.fill((0, 0, 0))
                    self.data_surface_filtered.fill((0, 0, 0))

        except Exception as e:
            print(f"디렉토리 데이터 수신 오류: {e}")

    def process_sector_data(self, sector_data):
        first_azimuth = float(sector_data[0][0])
        current_sector = int(first_azimuth // 30)
        self.current_end_range = float(sector_data[0][1])
        
        for row in sector_data:
            azimuth = float(row[0])
            timestamp = int(row[2])
            intensity_data = np.array([int(x) for x in row[3:]])
            
            if timestamp > self.sector_timestamps[current_sector]:
                if current_sector != self.angle_idx:
                    self.clear_sector(current_sector)
                    self.angle_idx = current_sector

                self.draw_intensity_data(azimuth, intensity_data)
        
        self.sector_timestamps[current_sector] = int(sector_data[-1][2])

    def clear_sector(self, sector):
        sector_mask = pygame.Surface(self.screen_size, pygame.SRCALPHA)
        sector_mask.fill((0, 0, 0, 0))
        
        start_angle = sector * 30
        end_angle = (sector + 1) * 30
        
        # 원본 레이더용 섹터 지우기
        points_original = [self.center_original]
        # 필터링된 레이더용 섹터 지우기
        points_filtered = [self.center_filtered]
        
        start_rad = math.radians(start_angle) - math.pi/2
        end_rad = math.radians(end_angle) - math.pi/2
        
        for angle in np.linspace(start_rad, end_rad, 31):
            x_orig = self.center_original[0] + (self.scale_factor * 1.2) * math.cos(angle)
            y_orig = self.center_original[1] + (self.scale_factor * 1.2) * math.sin(angle)
            points_original.append((int(x_orig), int(y_orig)))
            
            x_filt = self.center_filtered[0] + (self.scale_factor * 1.2) * math.cos(angle)
            y_filt = self.center_filtered[1] + (self.scale_factor * 1.2) * math.sin(angle)
            points_filtered.append((int(x_filt), int(y_filt)))
        
        if len(points_original) > 2:
            pygame.draw.polygon(sector_mask, (0, 0, 0, 255), points_original)
            pygame.draw.polygon(sector_mask, (0, 0, 0, 255), points_filtered)
        
        self.data_surface_original.blit(sector_mask, (0, 0))
        self.data_surface_filtered.blit(sector_mask, (0, 0))

    def draw_intensity_data(self, azimuth, intensity_data):
        # 원본 데이터 그리기
        self._draw_single_intensity_data(self.data_surface_original, self.center_original, azimuth, intensity_data)
        
        # 필터링된 데이터 처리 및 그리기
        filtered_data = self.apply_filter(intensity_data)
        self._draw_single_intensity_data(self.data_surface_filtered, self.center_filtered, azimuth, filtered_data)

    def _draw_single_intensity_data(self, surface, center, azimuth, intensity_data):
        pixel_array = pygame.surfarray.pixels2d(surface)
        # 현재 스케일에 맞춘 최대 거리 계산
        max_range = self.current_end_range * (self.concentric_circles-self.scale) / self.concentric_circles
        ranges = np.linspace(0.0, self.current_end_range, len(intensity_data))
        
        # 스케일 범위 내의 데이터만 선택
        range_mask = ranges <= max_range
        intensity_mask = intensity_data > 0
        mask = range_mask & intensity_mask
        
        if np.any(mask):
            theta = math.radians(azimuth)
            scaled_ranges = ranges[mask] * self.scale_factor / max_range
            
            x = (scaled_ranges * np.cos(theta - math.pi/2) + center[0]).astype(int)
            y = (scaled_ranges * np.sin(theta - math.pi/2) + center[1]).astype(int)
            
            valid_indices = (x >= 0) & (x < self.screen_size[0]) & (y >= 0) & (y < self.screen_size[1])
            x = x[valid_indices]
            y = y[valid_indices]
            intensities = intensity_data[mask][valid_indices]
            
            colors = np.minimum(intensities, 255) << 8
            pixel_array[x, y] = colors
        
        del pixel_array

    def draw_progress_bar(self):
        """진행 상황 스크롤바 그리기"""
        if self.mode == 'directory' and self.total_files > 0:
            # 스크롤바 배경
            pygame.draw.rect(self.screen, (50, 50, 50), self.scroll_rect)
            
            # 스크롤 버튼
            button_width = max(20, (self.scroll_rect.width - 4) / self.total_files)
            button_x = self.scroll_rect.left + (self.scroll_rect.width - button_width) * (self.current_file_index / max(1, self.total_files - 1))
            self.scroll_button_rect = pygame.Rect(button_x, self.scroll_rect.top, button_width, self.scroll_rect.height)
            pygame.draw.rect(self.screen, (0, 150, 0), self.scroll_button_rect)
            
            # 진행률 텍스트
            font = pygame.font.Font(None, 24)
            progress_text = f"{self.current_file_index}/{self.total_files}"
            text_surface = font.render(progress_text, True, (0, 150, 0))
            text_rect = text_surface.get_rect(midtop=(self.scroll_rect.centerx, self.scroll_rect.top - 25))
            self.screen.blit(text_surface, text_rect)
            
            # 재생/정지 상태 표시
            status_text = "Paused" if self.is_paused else "Playing"
            status_surface = font.render(status_text, True, (0, 150, 0))
            status_rect = status_surface.get_rect(midtop=(self.scroll_rect.centerx, self.scroll_rect.top - 45))
            self.screen.blit(status_surface, status_rect)

    def handle_scroll_events(self, event):
        """스크롤바 이벤트 처리"""
        if self.mode != 'directory':
            return
            
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                self.is_paused = not self.is_paused
            # 왼쪽 방향키 처리
            elif event.key == pygame.K_LEFT:
                new_index = max(0, self.current_file_index - 5)
                if new_index != self.current_file_index:
                    self.current_file_index = new_index
                    self.data_surface_original.fill((0, 0, 0))
                    self.data_surface_filtered.fill((0, 0, 0))
            # 오른쪽 방향키 처리
            elif event.key == pygame.K_RIGHT:
                new_index = min(self.total_files - 1, self.current_file_index + 5)
                if new_index != self.current_file_index:
                    self.current_file_index = new_index
                    self.data_surface_original.fill((0, 0, 0))
                    self.data_surface_filtered.fill((0, 0, 0))
             
            elif event.key == pygame.K_7:
                if self.concentric_circles > 1:
                    self.concentric_circles -= 1
            elif event.key == pygame.K_8:
                self.concentric_circles += 1
            elif event.key == pygame.K_9:
                if self.scale < self.concentric_circles - 1:
                    self.scale += 1
            elif event.key == pygame.K_0:
                if self.scale > 0:
                    self.scale -= 1
               
        
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1 and self.scroll_button_rect.collidepoint(event.pos):
                self.dragging = True
        elif event.type == pygame.MOUSEBUTTONUP:
            if event.button == 1:
                self.dragging = False
        elif event.type == pygame.MOUSEMOTION and self.dragging:
            rel_x = event.pos[0] - self.scroll_rect.left
            progress = max(0, min(1, rel_x / self.scroll_rect.width))
            new_index = int(progress * (self.total_files - 1))
            if new_index != self.current_file_index:
                self.current_file_index = new_index
                # 데이터 서피스 초기화
                self.data_surface_original.fill((0, 0, 0))
                self.data_surface_filtered.fill((0, 0, 0))

    def run(self):
        if self.mode == 'live':
            self.process = subprocess.Popen(['./src/SPxLiveStream', '-a', '239.192.43.79'],
                                          stdout=subprocess.PIPE,
                                          stderr=subprocess.PIPE,
                                          universal_newlines=True)
            self.receiver_thread = threading.Thread(target=self.data_receiver)
        elif self.mode == 'file':
            # 단일 파일 모드로 실행
            self.process = subprocess.Popen(['./src/SPxDataStream', self.file_path],
                                          stdout=subprocess.PIPE,
                                          stderr=subprocess.PIPE,
                                          universal_newlines=True)
            self.receiver_thread = threading.Thread(target=self.data_receiver)
        elif self.mode == 'directory':
            # 디렉토리 모드로 실행
            self.receiver_thread = threading.Thread(target=self.data_receiver_directory)
        else:
            raise ValueError("잘못된 모드입니다. 'live', 'file', 또는 'directory' 중 하나를 선택하세요.")

        self.receiver_thread.daemon = True
        self.receiver_thread.start()
        
        clock = pygame.time.Clock()
        
        try:
            while self.running:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        self.running = False
                    self.handle_scroll_events(event)
                
                while not self.data_queue.empty():
                    sector_data, receive_time = self.data_queue.get()
                    processing_delay = time.time() - receive_time
                    
                    # 일시 정지 상태가 아닐 때만 처리 지연 메시지 표시
                    if processing_delay > 0.1 and not self.is_paused:
                        print(f"처리 지연 감지: {processing_delay:.3f}초")
                    
                    self.process_sector_data(sector_data)
                
                self.screen.fill((0, 0, 0))
                self.draw_radar_display()
                self.screen.blit(self.data_surface_original, (0, 0), special_flags=pygame.BLEND_ADD)
                self.screen.blit(self.data_surface_filtered, (0, 0), special_flags=pygame.BLEND_ADD)
                
                if self.mode == 'directory':
                    self.draw_progress_bar()
                
                pygame.display.flip()
                
                if self.current_sector == 0 and self.angle_idx == 11:
                    self.sector_timestamps = {i: 0 for i in range(12)}
                
                clock.tick(60)

        except KeyboardInterrupt:
            self.cleanup()
        finally:
            self.cleanup()

    def cleanup(self):
        if self.process:
            self.process.terminate()
            self.process.wait()
        pygame.quit()


    def apply_filter(self, intensity_data):
        """레이더 데이터에 필터를 적용하는 메서드"""
        filtered_data = intensity_data.copy()
        
        # === 필터링 알고리즘 시작 ===
        filtered_data[:150] = 0
        
        # 여기에 추가 필터링 알고리즘
        # - 노이즈 제거
        # - 신호 증폭
        # - 특정 범위 필터링
        # - 이동 평균 필터
        # 등등...
        
        # === 필터링 알고리즘 끝 ===
        
        return filtered_data


class Mode(Enum):
    LIVE = 'live'
    FILE = 'file'
    DIRECTORY = 'directory'
    
    @staticmethod
    def from_string(s):
        try:
            return Mode[s.upper()]
        except KeyError:
            raise ValueError(f"Invalid mode: {s}")

if __name__ == "__main__":
    
    mode = Mode.DIRECTORY
    # Mode.LIVE: 실시간 레이더 데이터 스트리밍 모드
    # Mode.FILE: SPx 레이더(*.cpr) 데이터 파일 재생 모드  
    # Mode.DIRECTORY: *.txt 로 변환된 레이더 데이터 파일들이 있는 디렉토리 재생 모드
    
    if mode == Mode.LIVE:
        radar = RadarHandler(mode='live')
    elif mode == Mode.FILE:
        radar = RadarHandler(mode='file', file_path='20250124-120122-0x2eea4790.cpr')
    elif mode == Mode.DIRECTORY:
        radar = RadarHandler(mode='directory', file_path='./data/20250124-120122-0x2eea4790')
    else:
        raise ValueError(f"Invalid mode: {mode}")

    radar.run()