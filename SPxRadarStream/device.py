import subprocess
import csv
from io import StringIO
import multiprocessing
import time
import glob

class RadarHandler:
    def __init__(self, global_vals, mode='live', file_path=None):
        self.global_vals = global_vals
        self.mode = mode
        self.file_path = file_path
        self.files = []
        self.run()

    def data_receiver(self):
        last_stats_time = time.time()
        buffer = [[] for _ in range(12)]
        current_sector = 0
        
        try:
            while self.global_vals.running:
                line = self.process.stdout.readline()
                if line:
                    try:
                        reader = csv.reader(StringIO(line.strip()))
                        for row in reader:
                            if len(row) >= 3:
                                azimuth = float(row[0])
                                sector_idx = int(azimuth // 30)
                                
                                if sector_idx != current_sector and buffer[current_sector]:
                                    if self.global_vals.data_queue.qsize() < 10:
                                        self.global_vals.data_queue.put((buffer[current_sector].copy(), time.time()))
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
            self.global_vals.total_files = len(self.files)
            
            if not self.files:
                print(f"경고: {search_pattern} 경로에서 파일을 찾을 수 없습니다.")
                return
            
            while self.global_vals.running:
                try:
                    # 파일 인덱스가 변경되었고 일시정지 상태일 때
                    if self.global_vals.is_paused and last_file_index != self.global_vals.current_file_index:
                        # 버퍼 초기화
                        buffer = [[] for _ in range(12)]
                        current_sector = 0
                        
                        # 현재 선택된 파일의 데이터 즉시 처리
                        file = self.files[self.global_vals.current_file_index]
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
                                        self.global_vals.data_queue.put((buffer[current_sector].copy(), time.time()))
                                        buffer[current_sector] = []
                                    
                                    current_sector = sector_idx
                                    buffer[sector_idx].append([str(azimuth), str(range_val), str(timestamp)] + intensity_data)
                                    
                                except Exception as e:
                                    print(f"데이터 파싱 오류: {e}")
                        
                        # 마지막 버퍼 데이터 처리
                        if buffer[current_sector]:
                            self.global_vals.data_queue.put((buffer[current_sector].copy(), time.time()))
                        
                        last_file_index = self.global_vals.current_file_index
                    
                    # 일시정지 상태일 때는 대기
                    if self.global_vals.is_paused:
                        time.sleep(0.1)
                        continue
                        
                    # 기존 실시간 재생 로직
                    file = self.files[self.global_vals.current_file_index]
                    with open(file, 'r') as f:
                        for line in f:
                            if not self.global_vals.running:
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
                                    if self.global_vals.data_queue.qsize() < 36:
                                        self.global_vals.data_queue.put((buffer[current_sector].copy(), time.time()))
                                        buffer[current_sector] = []
                                    else:
                                        print("큐가 가득 찼습니다. 데이터 스킵")
                                        buffer[current_sector] = []
                                
                                current_sector = sector_idx
                                buffer[sector_idx].append([str(azimuth), str(range_val), str(timestamp)] + intensity_data)
                                
                            except Exception as e:
                                print(f"데이터 파싱 오류: {e}")
                        
                            time.sleep(0.0002)  # 데이터 처리 속도 조절
                    
                    last_file_index = self.global_vals.current_file_index
                    self.global_vals.current_file_index += 1
                    
                    # 파일의 끝에 도달하면 처음으로 돌아가기
                    if self.global_vals.current_file_index >= self.global_vals.total_files:
                        self.global_vals.current_file_index = 0
                        # 데이터 서피스 초기화
                        # self.data_surface_original.fill((0, 0, 0))
                        # self.data_surface_filtered.fill((0, 0, 0))
                except:
                    pass

        except Exception as e:
            # pass
            print(f"디렉토리 데이터 수신 오류: {e}")


    
    def run(self):
        if self.mode == 'live':
            self.process = subprocess.Popen(['./src/SPxLiveStream', '-a', '239.192.43.79'],
                                          stdout=subprocess.PIPE,
                                          stderr=subprocess.PIPE,
                                          universal_newlines=True)
            
            self.receiver_thread = multiprocessing.Process(target=self.data_receiver)
        elif self.mode == 'file':
            # 단일 파일 모드로 실행
            self.process = subprocess.Popen(['./src/SPxDataStream', self.file_path],
                                          stdout=subprocess.PIPE,
                                          stderr=subprocess.PIPE,
                                          universal_newlines=True)
            self.receiver_thread = multiprocessing.Process(target=self.data_receiver)
        elif self.mode == 'directory':
            # 디렉토리 모드로 실행
            
            self.receiver_thread = multiprocessing.Process(target=self.data_receiver_directory)
        else:
            raise ValueError("잘못된 모드입니다. 'live', 'file', 또는 'directory' 중 하나를 선택하세요.")

        # self.receiver_thread.daemon = True
        self.receiver_thread.start()
