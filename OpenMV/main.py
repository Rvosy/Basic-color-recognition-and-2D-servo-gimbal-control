import sensor, image, time, math

# 定义颜色阈值
thresholds = [
    (0, 100, 19, 54, 9, 33),    # 红色阈值
    (0, 100, -61, -29, 0, 36),  # 绿色阈值
    (0, 100, -32, -5, -41, -4)  # 蓝色阈值
]

sensor.reset()  # 初始化传感器
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)  # 设置分辨率为QVGA
sensor.skip_frames(10)  # 让新的设置生效
sensor.set_auto_whitebal(False)  # 关闭自动白平衡
clock = time.clock()  # 跟踪FPS帧率

# 计算两个矩形的IoU
def iou(rect1, rect2):
    x1, y1, w1, h1 = rect1
    x2, y2, w2, h2 = rect2
    
    inter_w = min(x1 + w1, x2 + w2) - max(x1, x2)
    inter_h = min(y1 + h1, y2 + h2) - max(y1, y2)
    if inter_w <= 0 or inter_h <= 0:
        return 0.0
    inter_area = inter_w * inter_h
    area1 = w1 * h1
    area2 = w2 * h2
    union_area = area1 + area2 - inter_area
    return inter_area / union_area

# 找最大色块并过滤掉小的噪声色块
def find_max(blobs):
    max_blob = None
    max_size = 0
    for blob in blobs:
        if blob.pixels() > max_size and blob.pixels() > 100:  # 过滤掉小于100像素的噪声色块
            max_blob = blob
            max_size = blob.pixels()
    return max_blob

# 简单的卡尔曼滤波器
class SimpleKalmanFilter:
    def __init__(self, process_noise, measurement_noise, estimated_error, initial_value):
        self.q = process_noise
        self.r = measurement_noise
        self.e = estimated_error
        self.x = initial_value

    def predict(self):
        self.e = self.e + self.q
        return self.x

    def update(self, measurement):
        k = self.e / (self.e + self.r)
        self.x = self.x + k * (measurement - self.x)
        self.e = (1 - k) * self.e

# 初始化追踪对象
tracked_blob = None
color_index = None  # 初始颜色索引为None
kf_x = None  # 卡尔曼滤波器，用于x坐标
kf_y = None  # 卡尔曼滤波器，用于y坐标
kf_w = None  # 卡尔曼滤波器，用于宽度
kf_h = None  # 卡尔曼滤波器，用于高度

while True:
    clock.tick()  # 追踪两个snapshots()之间经过的毫秒数
    img = sensor.snapshot()  # 拍一张照片并返回图像

    if tracked_blob is None:
        # 如果没有追踪对象，则重新寻找所有颜色
        for i, threshold in enumerate(thresholds):
            blobs = img.find_blobs([threshold])
            if blobs:
                max_blob = find_max(blobs)
                if max_blob:
                    tracked_blob = max_blob
                    color_index = i
                    # 初始化卡尔曼滤波器
                    kf_x = SimpleKalmanFilter(1, 5, 1, max_blob.cx())
                    kf_y = SimpleKalmanFilter(1, 5, 1, max_blob.cy())
                    kf_w = SimpleKalmanFilter(1, 5, 1, max_blob.w())
                    kf_h = SimpleKalmanFilter(1, 5, 1, max_blob.h())
                    break  # 找到第一个合适的颜色斑点后就停止寻找

    else:
        # 更新追踪对象
        blobs = img.find_blobs([thresholds[color_index]])
        max_iou = 0
        best_blob = None

        for blob in blobs:
            iou_value = iou(tracked_blob.rect(), blob.rect())
            if iou_value > max_iou:
                max_iou = iou_value
                best_blob = blob

        if max_iou >= 0.5:
            tracked_blob = best_blob
            kf_x.update(tracked_blob.cx())
            kf_y.update(tracked_blob.cy())
            kf_w.update(tracked_blob.w())
            kf_h.update(tracked_blob.h())
        else:
            predicted_rect = (int(kf_x.predict() - kf_w.predict() / 2),
                              int(kf_y.predict() - kf_h.predict() / 2),
                              int(kf_w.predict()),
                              int(kf_h.predict()))
            max_iou = 0
            best_blob = None

            for blob in blobs:
                iou_value = iou(predicted_rect, blob.rect())
                if iou_value > max_iou:
                    max_iou = iou_value
                    best_blob = blob

            if max_iou >= 0.5:
                tracked_blob = best_blob
                kf_x.update(tracked_blob.cx())
                kf_y.update(tracked_blob.cy())
                kf_w.update(tracked_blob.w())
                kf_h.update(tracked_blob.h())
            else:
                tracked_blob = None  # 如果追踪对象丢失，则重置追踪对象
                color_index = None
                kf_x = None
                kf_y = None
                kf_w = None
                kf_h = None

    if tracked_blob:
        # 卡尔曼滤波器预测
        predicted_x = kf_x.predict()
        predicted_y = kf_y.predict()
        predicted_w = kf_w.predict()
        predicted_h = kf_h.predict()
        predicted_rect = (int(predicted_x - predicted_w / 2),
                          int(predicted_y - predicted_h / 2),
                          int(predicted_w),
                          int(predicted_h))
        # 绘制追踪对象
        color = (255, 0, 0) if color_index == 0 else (0, 255, 0) if color_index == 1 else (0, 0, 255)
        img.draw_rectangle(tracked_blob.rect(), color=color)  # 用矩形标记出目标颜色区域
        img.draw_cross(tracked_blob.cx(), tracked_blob.cy(), color=color)  # 在目标颜色区域的中心画十字形标记
        # 绘制卡尔曼滤波预测矩形框
        img.draw_rectangle(predicted_rect, color=(255, 255, 255))  # 用白色矩形标记预测区域
        print(f"Tracked blob at: ({tracked_blob.cx()}, {tracked_blob.cy()}) with color index {color_index}")

    print(clock.fps())  # 输出当前帧率
