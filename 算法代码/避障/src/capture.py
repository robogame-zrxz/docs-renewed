import cv2
import numpy as np

def get_distances_to_squares(image, actual_square_size):
    # 转换为灰度图像
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # 使用Canny算法进行边缘检测
    edges = cv2.Canny(gray, 50, 150)

    # 查找轮廓
    contours, _ = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    squares = []
    for cnt in contours:
        # 计算轮廓的面积
        area = cv2.contourArea(cnt)

        # 计算轮廓的周长
        perimeter = cv2.arcLength(cnt, True)

        # 使用多边形逼近轮廓，得到多边形的顶点
        approx = cv2.approxPolyDP(cnt, 0.02 * perimeter, True)

        # 如果顶点数量为4，并且面积大于一定值，那么就认为找到了一个正方形
        if len(approx) == 4 and area > 1000:
            squares.append(approx)

    # 按面积大小排序，取最大的两个
    squares = sorted(squares, key=cv2.contourArea, reverse=True)[:2]

    # 计算距离
    distances = []
    for square in squares:
        _, _, w, h = cv2.boundingRect(square)
        # 估计距离
        distance = actual_square_size / max(w, h)
        distances.append(distance)

    return distances
