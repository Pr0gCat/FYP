import torch
import cv2

# Model
model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True)

cap = cv2.VideoCapture(1 + cv2.CAP_DSHOW)
Screen_Weight = 720
Screem_Height = 480
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(3, Screen_Weight)
cap.set(4, Screem_Height)

# Images
imgs = ['download.jpg']  # batch of images
# imgs = cap.read()
# Inference
results = model(imgs)

# Results
# results.print()
# results.show()
print(results.pandas().xyxy)
data_list = [] 
for counter in range(len(results.xyxy[0])):
    data_dict = {"xmax": results.pandas().xyxy[0].xmax[counter],
                 "xmin": results.pandas().xyxy[0].xmin[counter], 
                 "ymax": results.pandas().xyxy[0].ymax[counter], 
                 "ymin": results.pandas().xyxy[0].ymin[counter], 
                 "confidence": results.pandas().xyxy[0].confidence[counter]}
    data_list.append(data_dict)