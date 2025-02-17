from dataclasses import dataclass
from enum import Enum
import multiprocessing

class Operation_mode(Enum):
    LIVE = 1
    FILE = 2
    DIRECTORY = 4

    @staticmethod
    def from_string(s):
        try:
            return Operation_mode[s.upper()]
        except KeyError as e:
            raise ValueError(f"{s} is not a valid Operation mode") from e

@dataclass
class SETTINGS:
    scale_factor: float
    reflectivity_filter: int
    parameter_path: str
    frame_width: int


def initialize_global_values(config):
    manager = multiprocessing.Manager()
    global_vals = manager.Namespace()
    global_vals.LiDAR_running = False
    global_vals.CAMERA_running = False