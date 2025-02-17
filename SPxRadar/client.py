from typing import Optional
from SPxRadar.config import SETTINGS, initialize_global_values, Operation_mode
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

class SetParameters:
    def __init__(self):
        self.settings: Optional[SETTINGS] = None


class Builder:
    def __init__(self,config = SetParameters()):
        self.config = config

    def set_camera_config(self, camera_parameters):
        self.config.Parameters = camera_parameters
        return self

    def settings(self,settings):
        self.config.settings = settings
        return self
    
    def initialize_global_values(self):
        self.global_vals = initialize_global_values(self.config)
        return self

    def build(self):
        if self.config.settings.Operation_mode == Operation_mode.LIVESTREAM:
            CameraHandler(self.global_vals, self.config)
            LiDARHandler(self.global_vals, self.config)
            FindCheckerboard(self.global_vals, self.config)
        CameraLiDARCalibration(self.global_vals, self.config)
        return self