class RadarFilter:
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