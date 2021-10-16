import torch
import cv2
import time
import os

# Model
dir =  os.path.abspath(os.getcwd()).replace('\\', '/')
dir = dir + '/yolov5'
model_path = dir + '/weights/best.pt'
model = torch.hub.load(dir, 'custom',  model_path,  source='local')
#model = torch.hub.load('ultralytics/yolov5', 'custom', path='weights/best.pt')
cap = cv2.VideoCapture(0 + cv2.CAP_DSHOW)
Screen_Weight = 720
Screem_Height = 480
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(3, Screen_Weight)
cap.set(4, Screem_Height)

# Images
# imgs = ['download.jpg']  # batch of images
# imgs = cap.read()

# Results
# results.print()
# results.show()
while True:
    ret, imgs  = cap.read()
    if ret:
        size = 0
        results = model(imgs)
        print(results.pandas().xyxy)
        data_list = [] 
        for counter in range(len(results.xyxy[0])):
            data_dict = {"xmax": results.pandas().xyxy[0].xmax[counter],
                        "xmin": results.pandas().xyxy[0].xmin[counter], 
                        "ymax": results.pandas().xyxy[0].ymax[counter], 
                        "ymin": results.pandas().xyxy[0].ymin[counter], 
                        "confidence": results.pandas().xyxy[0].confidence[counter]}
            box_size = abs(data_dict['xmax'] - data_dict['xmin']) * abs(data_dict['ymax'] - data_dict['ymin'])
            data_list.append(data_dict)
            if box_size > size:
                size = box_size
                chosen = data_dict