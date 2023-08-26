import cv2
import serial
from capture import get_distances_to_squares

# 创建一个串行通信对象
ser = serial.Serial("/dev/ttyS0", 9600)

# 创建一个视频捕获对象
cap = cv2.VideoCapture(0)

# 定义车子的宽度和场地的边界
car_width = 50  # 车宽
field_boundary = 100  # 避障区域宽度
box_size = [10, 10]

# 定义变量来储存移动的距离


# 定义变量来储存离左边和右边的距离
distance_to_left = 0
distance_to_right = field_boundary - car_width

move_distances = []

for _ in range(2):
    # 捕获一帧图像
    ret, frame = cap.read()
    if not ret:
        print("Failed to capture frame")
        break

    # 检测障碍物
    distances = get_distances_to_squares(frame, box_size)

    x1 = distances[0, 0]
    x2 = distances[1, 0]

    if (field_boundary - distance_to_left - x1) > car_width:
        move_distance = x1

    elif (field_boundary - distance_to_left - x2) > car_width:
        move_distance = x2

    else:
        move_distance = (x2 - x1) / 2

    if move_distance <= 0:
        send_move_left_command_to_stm32(ser, abs(move_distance))
    else:
        send_move_right_command_to_stm32(ser, move_distance)
    distance_to_left += move_distance
    distance_to_right -= move_distance



    move_distances.append(move_distance)

    # 前进到下一排障碍物
    send_forward_command_to_stm32(ser, move_distance)
