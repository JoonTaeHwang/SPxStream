import os
from SPxRadarStream.config import initialize_global_values, SETTINGS, Mode
from typing import Optional
from SPxRadarStream.device import RadarHandler, LiDARHandler
from SPxRadarStream.display import RadarDisplay

class SetParameters:
    def __init__(self):
        self.settings: Optional[SETTINGS] = None

class Builer:
    def __init__(self,config = SetParameters):
        self.config = config
    
    def settings(self,settings):
        self.config.settings = settings
        return self
    
    def initialize_global_values(self):
        self.global_vals = initialize_global_values()
        return self
    
    def build(self):
        
        if self.config.settings.mode == Mode.LIVE:
            RadarDisplay(self.global_vals, self.config, mode='live')
        elif self.config.settings.mode == Mode.FILE:
            file_path = os.path.join(os.path.dirname(os.path.dirname(__file__)), '20250124-120122-0x2eea4790.cpr')
            RadarDisplay(self.global_vals, self.config, mode='file', file_path=file_path)
        elif self.config.settings.mode == Mode.DIRECTORY:
            # 현재 스크립트의 상위 폴더 경로 지정
            radar_dir = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'data/20250124-120122-0x2eea4790')
            lidar_dir = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'data/FVMdata')
            RadarHandler(self.global_vals, mode='directory', file_path=radar_dir)
            LiDARHandler(self.global_vals, mode='directory',file_path=lidar_dir)
            RadarDisplay(self.global_vals, self.config, mode='directory', file_path=radar_dir)
        else:
            raise ValueError(f"Invalid mode: {self.config.settings.mode}")


