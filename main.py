from SPxRadarStream.client import Builer
from SPxRadarStream.config import Mode, SETTINGS
import os

if __name__ == "__main__":
    
    setting = SETTINGS(
        mode=Mode.DIRECTORY
        )
    # mode = Mode.DIRECTORY
    # Mode.LIVE: 실시간 레이더 데이터 스트리밍 모드
    # Mode.FILE: SPx 레이더(*.cpr) 데이터 파일 재생 모드  
    # Mode.DIRECTORY: *.txt 로 변환된 레이더 데이터 파일들이 있는 디렉토리 재생 모드
    
    RadarBuiler = (
        Builer()
        .settings(setting)
        .initialize_global_values()
        .build()
    )