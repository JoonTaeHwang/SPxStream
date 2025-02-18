from dataclasses import dataclass
from enum import Enum
import multiprocessing

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
        
@dataclass
class SETTINGS:
    mode: Mode
        
def initialize_global_values():
    manager = multiprocessing.Manager()
    global_vals = manager.Namespace()
    global_vals.angle = None
    global_vals.sector = None
    global_vals.timestamp = None
    global_vals.radar_data = None
    global_vals.intensity_data = None
    global_vals.filtered_data = None
    # Add a Queue for data
    global_vals.Radar_queue = manager.Queue()
    global_vals.LiDAR_queue = manager.Queue()
    global_vals.LiDAR_PCD = None
    global_vals.running = True
    global_vals.is_paused = False
    global_vals.current_file_index = 0
    global_vals.total_files = 0
    
    return global_vals