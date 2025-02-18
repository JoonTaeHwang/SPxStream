import numpy as np
import pygame
import math
import time
from SPxRadarStream.filter import RadarFilter

class RadarDisplay:
    def __init__(
                self,
                global_vals,
                Config,
                screen_size=(800, 800),
                mode='live',
                file_path=None
                ):
        pygame.init()
        self.global_vals = global_vals
        self.config = Config
        self.screen_size = screen_size
        self.screen = pygame.display.set_mode(screen_size)
        self.radar_filter = RadarFilter()
        # 모드에 따른 캡션 설정
        mode_text = {
            'live': 'LiveStream',
            'file': 'fileStream',
            'directory': 'directoryStream'
        }.get(mode, '알 수 없는 모드')
        pygame.display.set_caption(f"Radar Display - Original vs Filtered ({mode_text})")
        
        self.center_original = (screen_size[0]//2, screen_size[1]//2)  # 왼쪽 레이더 중심
        self.center_filtered = (3*screen_size[0]//2, screen_size[1]//2)  # 오른쪽 레이더 중심
        self.scale_factor = 350
        self.current_end_range = 50.0
        self.scale = 0
        self.concentric_circles = 5  # 동심원 개수

        self.angle_idx = 0
        self.current_sector = 0
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
        self.scroll_rect = pygame.Rect(50, self.screen_size[1] - 30, self.screen_size[0] - 100, 20)
        self.scroll_button_rect = pygame.Rect(50, self.screen_size[1] - 30, 20, 20)
        self.dragging = False
        self.display_mode = 'single'  # 'single' 또는 'dual' 모드
        
        self.display_lidar_data = False
        
        self.run()

    def draw_radar_display(self):
        if self.display_mode == 'single':
            # 단일 레이더 (원본만)
            self.draw_single_radar((self.screen_size[0]//2, self.screen_size[1]//2), "Original")
        else:
            # 두 개의 레이더 (원본과 필터링)
            self.draw_single_radar(self.center_original, "Original")
            self.draw_single_radar(self.center_filtered, "Filtered")
        
        # LiDAR 데이터 그리기
        if  self.display_lidar_data:
            self.draw_lidar_data()

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

    def process_sector_data(self, sector_data, received_time):
        first_azimuth = float(sector_data[0][0])
        current_sector = int(first_azimuth // 30)
        self.current_end_range = float(sector_data[0][1])
        
        for row in sector_data:
            azimuth = float(row[0])
            intensity_data = np.array([int(x) for x in row[3:]])
            
            if received_time > self.sector_timestamps[current_sector]:
                if current_sector != self.angle_idx:
                    self.clear_sector(current_sector)
                    self.angle_idx = current_sector

                self.draw_intensity_data(azimuth, intensity_data)
        
        self.sector_timestamps[current_sector] = received_time#int(sector_data[-1][2])

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
        if self.display_mode == 'single':
            # 원본 데이터 그리기
            self._draw_single_intensity_data(self.data_surface_original, self.center_original, azimuth, intensity_data)
        elif self.display_mode == 'filter_visualization':
            # 필터링 시각화 모드
            filtered_data = self.radar_filter.apply_filter(intensity_data)
            # 필터링된 데이터는 초록색으로 표시
            self._draw_single_intensity_data_colored(self.data_surface_original, self.center_original, azimuth, filtered_data, (0, 255, 0))
            # 필터링으로 제거된 데이터는 빨간색으로 표시
            removed_data = np.where(intensity_data > filtered_data, intensity_data, 0)
            self._draw_single_intensity_data_colored(self.data_surface_original, self.center_original, azimuth, removed_data, (122, 122, 122))
        else:
            # 듀얼 모드 (기존 코드)
            filtered_data = self.radar_filter.apply_filter(intensity_data)
            
            self._draw_single_intensity_data(self.data_surface_original, self.center_original, azimuth, intensity_data)
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

    def _draw_single_intensity_data_colored(self, surface, center, azimuth, intensity_data, color):
        pixel_array = pygame.surfarray.pixels2d(surface)
        max_range = self.current_end_range * (self.concentric_circles-self.scale) / self.concentric_circles
        ranges = np.linspace(0.0, self.current_end_range, len(intensity_data))
        
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
            
            # RGB 색상 변환
            r, g, b = color
            colors = np.zeros_like(x, dtype=np.uint32)
            if r: colors |= (r << 16)
            if g: colors |= (g << 8)
            if b: colors |= b
            
            pixel_array[x, y] = colors
        
        del pixel_array

    def draw_progress_bar(self):
        """진행 상황 스크롤바 그리기"""
        if self.mode == 'directory' and self.global_vals.total_files > 0:
            # 디스플레이 모드에 따라 스크롤바 위치 조정
            if self.display_mode == 'single':
                scroll_width = self.screen_size[0] - 100
                scroll_x = 50
            else:  # dual 모드
                scroll_width = self.screen_size[0] - 100
                scroll_x = 50

            # 스크롤바 위치 업데이트
            self.scroll_rect = pygame.Rect(scroll_x, self.screen_size[1] - 30, scroll_width, 20)
            
            # 스크롤바 배경
            pygame.draw.rect(self.screen, (50, 50, 50), self.scroll_rect)
            
            # 스크롤 버튼
            button_width = max(20, (self.scroll_rect.width - 4) / self.global_vals.total_files)
            button_x = self.scroll_rect.left + (self.scroll_rect.width - button_width) * (self.global_vals.current_file_index / max(1, self.global_vals.total_files - 1))
            self.scroll_button_rect = pygame.Rect(button_x, self.scroll_rect.top, button_width, self.scroll_rect.height)
            pygame.draw.rect(self.screen, (0, 150, 0), self.scroll_button_rect)
            
            # 진행률 텍스트
            font = pygame.font.Font(None, 24)
            progress_text = f"{self.global_vals.current_file_index}/{self.global_vals.total_files}"
            text_surface = font.render(progress_text, True, (0, 150, 0))
            text_rect = text_surface.get_rect(midtop=(self.scroll_rect.centerx, self.scroll_rect.top - 25))
            self.screen.blit(text_surface, text_rect)
            
            # 재생/정지 상태 표시
            status_text = "Paused" if self.global_vals.is_paused else "Playing"
            status_surface = font.render(status_text, True, (0, 150, 0))
            status_rect = status_surface.get_rect(midtop=(self.scroll_rect.centerx, self.scroll_rect.top - 45))
            self.screen.blit(status_surface, status_rect)

    def handle_scroll_events(self, event):
        """스크롤바 이벤트 처리"""
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_1:
                # 단일 레이더 모드로 전환 (원본 데이터)
                self.display_mode = 'single'
                self.screen_size = (800, 800)
                self.screen = pygame.display.set_mode(self.screen_size)
                self.data_surface_original = pygame.Surface(self.screen_size)
                self.data_surface_original.fill((0, 0, 0))
                self.data_surface_filtered = pygame.Surface(self.screen_size)
                self.data_surface_filtered.fill((0, 0, 0))
            elif event.key == pygame.K_2:
                # 단일 레이더 모드로 전환 (필터링 시각화)
                self.display_mode = 'filter_visualization'
                self.screen_size = (800, 800)
                self.screen = pygame.display.set_mode(self.screen_size)
                self.data_surface_original = pygame.Surface(self.screen_size)
                self.data_surface_original.fill((0, 0, 0))
                self.data_surface_filtered = pygame.Surface(self.screen_size)
                self.data_surface_filtered.fill((0, 0, 0))
            elif event.key == pygame.K_3:
                # 듀얼 레이더 모드로 전환
                self.display_mode = 'dual'
                self.screen_size = (1600, 800)
                self.screen = pygame.display.set_mode(self.screen_size)
                self.data_surface_original = pygame.Surface(self.screen_size)
                self.data_surface_original.fill((0, 0, 0))
                self.data_surface_filtered = pygame.Surface(self.screen_size)
                self.data_surface_filtered.fill((0, 0, 0))
            elif event.key == pygame.K_4:
                self.display_lidar_data = not self.display_lidar_data
            elif event.key == pygame.K_SPACE:
                self.global_vals.is_paused = not self.global_vals.is_paused
            # 왼쪽 방향키 처리
            elif event.key == pygame.K_LEFT:
                new_index = max(0, self.global_vals.current_file_index - 5)
                if new_index != self.global_vals.current_file_index:
                    self.global_vals.current_file_index = new_index
                    self.data_surface_original.fill((0, 0, 0))
                    self.data_surface_filtered.fill((0, 0, 0))
            # 오른쪽 방향키 처리
            elif event.key == pygame.K_RIGHT:
                new_index = min(self.global_vals.total_files - 1, self.global_vals.current_file_index + 5)
                if new_index != self.global_vals.current_file_index:
                    self.global_vals.current_file_index = new_index
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
            new_index = int(progress * (self.global_vals.total_files - 1))
            if new_index != self.global_vals.current_file_index:
                self.global_vals.current_file_index = new_index
                # 데이터 서피스 초기화
                self.data_surface_original.fill((0, 0, 0))
                self.data_surface_filtered.fill((0, 0, 0))

    def run(self):
        clock = pygame.time.Clock()
        
        try:
            while self.global_vals.running:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        self.global_vals.running = False
                    self.handle_scroll_events(event)
                
                while not self.global_vals.Radar_queue.empty():
                    sector_data, receive_time = self.global_vals.Radar_queue.get()
                    processing_delay = time.time() - receive_time
                    
                    # 일시 정지 상태가 아닐 때만 처리 지연 메시지 표시
                    if processing_delay > 0.11 and not self.global_vals.is_paused:
                        print(f"처리 지연 감지: {processing_delay:.3f}초")
                    
                    self.process_sector_data(sector_data,receive_time)
                
                self.screen.fill((0, 0, 0))
                self.draw_radar_display()
                self.screen.blit(self.data_surface_original, (0, 0), special_flags=pygame.BLEND_ADD)
                self.screen.blit(self.data_surface_filtered, (0, 0), special_flags=pygame.BLEND_ADD)
                
                if self.mode == 'directory':
                    # while self.global_vals.total_files==0:
                    #     time.sleep(0.01)
                    # time.sleep(0.1)
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

    def draw_lidar_data(self):
        """LiDAR 데이터를 화면에 그리기"""
        if not self.global_vals.LiDAR_queue.empty():
            pcd, timestamp = self.global_vals.LiDAR_queue.get()
            # 현재 스케일에 맞춘 최대 거리 계산
            max_range = self.current_end_range * (self.concentric_circles - self.scale) / self.concentric_circles
            scale_factor = self.scale_factor / max_range
            
            # 투명한 서피스 생성
            # lidar_surface = pygame.Surface(self.screen_size, pygame.SRCALPHA)
            
            for point in pcd:
                x, y, z, intensity = point
                
                # 스케일 범위 내의 점들만 그리기
                if abs(x) <= max_range and abs(y) <= max_range:
                    # z 축을 무시하고 x, y 좌표를 사용하여 그리기
                    # 0도를 정면으로 기준으로 그리기 위해 x, y를 변환
                    screen_x = int(self.center_original[0] + x * scale_factor)
                    screen_y = int(self.center_original[1] - y * scale_factor)

                    # intensity를 빨간색 강도로 변환 (0-255 범위)
                    color_intensity = min(max(intensity, 0), 255)
                    # 투명도 50 적용
                    # color = (color_intensity, 0, 0)  # 빨간색
                    color = (255, 0, 0)  # 빨간색
                    
                    # 화면에 점 그리기
                    pygame.draw.circle(self.screen, color, (screen_x, screen_y), 1)
                    
                    if self.display_mode == 'dual':
                        screen2_x = int(self.center_filtered[0] + x * scale_factor)
                        screen2_y = int(self.center_filtered[1] - y * scale_factor)
                        color = (255, 0, 0)  # 빨간색
                        pygame.draw.circle(self.screen, color, (screen2_x, screen2_y), 1)
                    
                    
            #         color = (255, 0, 0, 50)  # 빨간색, 투명도 50
                    
            #         # 투명한 서피스에 점 그리기
            #         pygame.draw.circle(lidar_surface, color, (screen_x, screen_y), 1)
            
            # # 메인 화면에 블릿
            # self.screen.blit(lidar_surface, (0, 0))
