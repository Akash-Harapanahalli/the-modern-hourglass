import numpy as np
import pandas as pd
# import cv2 as cv
# from google.colab.patches import cv2_imshow # for image display
# from skimage import io
# from PIL import Image
import matplotlib.pylab as plt
from datetime import datetime
import serial as pyser
import json

def distance(curr, next):
  return (curr[0]-next[0])**2 + (curr[1]-next[1])**2

def interpolate_path(waypoints):
    x = []
    y = []
    for point in waypoints:
        x.append(point[1])
        y.append(point[0])

    tck, *rest = interpolate.splprep([x,y])
    u = np.linspace(0,1, num=400)
    smooth = interpolate.splev(u,tck)

    return smooth

def simulate_tracing(points, freq):
    points = np.array(points)
    plt.figure(1)
    plt.xlim([0,np.max(points[:,1])])
    plt.ylim([0,np.max(points[:,0])])
    x_smooth, y_smooth = interpolate_path(points)
    for i in range(len(x_smooth)):
        plt.plot(x_smooth[i], y_smooth[i], color='blue', marker='o')
        plt.pause(1/freq)

def trace_num(number, last_x, coordinate_lookup, total_path_coords, x_scale, y_scale, number_spacing):
  image_path = '/content/%d_inv.png' % (number)
  # Read the image as a grayscale image
  img = cv.imread(image_path, 0)

  # Threshold the image
  ret,img = cv.threshold(img, 127, 255, 0)

  # Step 1: Create an empty skeleton
  size = np.size(img)
  skel = np.zeros(img.shape, np.uint8)

  # Get a Cross Shaped Kernel
  element = cv.getStructuringElement(cv.MORPH_CROSS, (3,3))

  # Repeat steps 2-4
  while True:
      #Step 2: Open the image
      open2 = cv.morphologyEx(img, cv.MORPH_OPEN, element)
      #Step 3: Substract open from the original image
      temp = cv.subtract(img, open2)
      #Step 4: Erode the original image and refine the skeleton
      eroded = cv.erode(img, element)
      skel = cv.bitwise_or(skel,temp)
      img = eroded.copy()
      # Step 5: If there are no white pixels left ie.. the image has been completely eroded, quit the loop
      if cv.countNonZero(img)==0:
          break

  # Displaying the final skeleton
  cv2_imshow(skel)
  cv.waitKey(0)
  cv.destroyAllWindows()

  rows,cols = np.nonzero(skel)

  # flips y axis
  for i in range(len(rows)):
    rows[i] = abs(120 - rows[i])
    cols[i] = cols[i]

  coords = [(float(rows[i]) * x_scale, float(cols[i]) * y_scale) for i in range(0, len(rows))]

  print('total number of coordinates: %d' % (len(coords)))

  # find vertex closest to (0,0)
  start_vertex = coords[0]
  for i in range (len(coords)):
    if (start_vertex[0]**2 + start_vertex[1]**2 > coords[i][0]**2 + coords[i][1]**2):
      start_vertex = coords[i]
  print('starting vertex: ' + str(start_vertex))

  # find vertex closest to end
  end_vertex = coords[0]
  for i in range (len(coords)):
    if (end_vertex[1]< coords[i][1]):
      end_vertex = coords[i]
  print('ending vertex: ' + str(end_vertex))

  # builds the ordered_coords list by checking neighbors
  visited = {start_vertex}
  ordered_coords = [start_vertex]

  curr_vertex = start_vertex
  for i in range (len(coords)):
    if (curr_vertex == end_vertex):
      ordered_coords.append(curr_vertex)
      break;
    next_vertex = (1000, 1000)
    for j in range (len(coords)):
      if (not(coords[j] in visited) and distance(next_vertex, curr_vertex) > distance(coords[j], curr_vertex)):
        next_vertex = coords[j]
    ordered_coords.append(next_vertex)

    total_vertex = (next_vertex[1] + last_x, next_vertex[0])
    total_path_coords.append(total_vertex)

    visited.add(next_vertex)
    curr_vertex = next_vertex

  ordered_coords = ordered_coords[1::5]

  print('simplified coordinates: %d' % (len(ordered_coords)))
  print('path for this number: ' + str(ordered_coords))
  coordinate_lookup[number] = ordered_coords


  # for i in range(len(ordered_coords)):
  #     plt.scatter((ordered_coords[i][1] + last_x), ordered_coords[i][0])

  last_x += end_vertex[1] + number_spacing

  return last_x

def create_dict(x_scale, y_scale, number_spacing):
  last_x = 0
  total_path_coords = []
  coordinate_lookup = {}

  for i in range(11):
    last_x = trace_num(i, last_x, coordinate_lookup, total_path_coords, x_scale, y_scale, number_spacing)

  # have to manually set path for 1 bc poor image quality
  one = [(6, 7), (4, 11), (5, 16), (8, 20), (12, 22), (17, 23), (20, 27), (21, 32), (21, 37), (22, 43), (27, 45), (32, 45), (37, 45), (42, 45), (47, 45), (52, 45), (57, 45), (62, 45), (67, 45), (72, 45), (77, 45), (82, 45), (87, 45), (92, 45), (97, 45), (101, 41), (102, 36), (101, 32), (104, 27), (109, 25), (113, 27), (116, 30), (118, 35), (120, 40), (120, 45), (118, 50), (114, 55), (109, 59), (104, 59), (99, 59), (94, 59), (89, 59), (84, 59), (79, 59), (74, 59), (69, 59), (64, 59), (59, 59), (54, 59), (49, 59), (44, 59), (39, 59), (34, 59), (29, 59), (24, 59), (21, 63), (21, 67), (21, 72), (20, 73), (20, 68), (18, 57), (101, 38), (101, 33), (102, 23), (18, 78), (16, 80), (12, 80), (8, 84), (6, 86), (3, 91), (6, 96)]
  scaled_one = []
  for i in range(len(one)):
    scaled_one.append((one[i][0] * x_scale, one[i][1] * y_scale))

  coordinate_lookup[1] = scaled_one

  print('\nlookup dict: ' + str(coordinate_lookup))

  # write total path to dictionary.json
  file = open('dictionary.json', 'w')
  file.write(json.dumps(coordinate_lookup))
  file.close()

# this function is mostly for fun
def time_path():
  currtime = datetime.now()
  hour = (currtime.hour - 4) % 12
  minute = currtime.minute
  # print(currtime.strftime("%H:%M:%S"))

  scale_factor = 2
  number_spacing = 0
  last_x = 0
  total_path_coords = []
  coordinate_lookup = {}
  digits = 1

  # time tracer
  last_x = trace_num(hour // 10, last_x, coordinate_lookup, total_path_coords, scale_factor, number_spacing)
  last_x = trace_num(hour % 10, last_x, coordinate_lookup, total_path_coords, scale_factor, number_spacing)
  last_x = trace_num(minute // 10, last_x, coordinate_lookup, total_path_coords, scale_factor, number_spacing)
  last_x = trace_num(minute % 10, last_x, coordinate_lookup, total_path_coords, scale_factor, number_spacing)

  print('\noverall path: ' + str(total_path_coords))

  # write total path to output
  file = open('output.txt', 'w')
  file.write(json.dumps(total_path_coords))
  file.close()

# sending discrete coordinates to serial port
def send_to_serial(list):
  # TODO: change serial port
  ser = pyser.Serial('/dev/ttyS1', 115200, timeout=3)
  for i in range(len(list)):
    answer = ser.read(1)
    if answer == 1:
      write(str(list[i]))        # write each coordinate
    else:
      print('timeout')
      i -= 1
    # if (ser.in_waiting != 0)

  ser.close()

def main():
  # reform dictionary to be safe and consider scale factor
  y_scale = 1
  x_scale = 1
  spacing = 0
  # amount of space between minute and hour numbers
  minute_separation = 0
  # create_dict(y_scale, x_scale, spacing)

  file = open('dictionary.json', 'r')
  path_dict = json.load(file)
  file.close()
  # print('test: ' + str(path_dict))

  currtime = datetime.now()
  hour = (12) if (currtime.hour == 12) else currtime.hour % 12
  minute = currtime.minute

  print('%d%d:%d%d' % (hour // 10, hour % 10, minute // 10, minute % 10))

  starting_point = (0.0, 0.0)

  time_trace = []

  first_digit = path_dict['{}'.format(hour // 10)]
  second_digit = path_dict['{}'.format(hour % 10)]
  colon = path_dict['10']
  third_digit = path_dict['{}'.format(minute // 10)]
  fourth_digit = path_dict['{}'.format(minute % 10)]

  # to manually test out times
  # first_digit = path_dict['1']
  # second_digit = path_dict['2']
  # colon = path_dict['10']
  # third_digit = path_dict['2']
  # fourth_digit = path_dict['1']

  new_first_digit = []
  new_second_digit = []
  new_colon = []
  new_third_digit = []
  new_fourth_digit = []

  last_x = starting_point[0]

  for i in range(len(first_digit)):
    new_first_digit.append((first_digit[i][0] + starting_point[1], first_digit[i][1] + starting_point[0]))
    time_trace.append(new_first_digit[i])

  last_x = new_first_digit[len(new_first_digit) - 1][1]

  for i in range(len(second_digit)):
    new_second_digit.append((second_digit[i][0] + starting_point[1], second_digit[i][1] + last_x))
    time_trace.append(new_second_digit[i])

  last_x = new_second_digit[len(new_second_digit) - 1][1] + minute_separation

  for i in range(len(colon)):
    new_colon.append((colon[i][0] + starting_point[1], colon[i][1] + last_x))
    time_trace.append(new_colon[i])

  last_x = new_colon[len(new_colon) - 1][1]

  for i in range(len(third_digit)):
    new_third_digit.append((third_digit[i][0] + starting_point[1], third_digit[i][1] + last_x))
    time_trace.append(new_third_digit[i])

  last_x = new_third_digit[len(new_third_digit) - 1][1]

  for i in range(len(fourth_digit)):
    new_fourth_digit.append((fourth_digit[i][0] + starting_point[1], fourth_digit[i][1] + last_x))
    time_trace.append(new_fourth_digit[i])

  # last_x = new_fourth_digit[len(new_fourth_digit) - 1][0]
  for i in range(len(time_trace)):
      plt.scatter(time_trace[i][1], time_trace[i][0])
      # make x coord go first and y coord go last
      time_trace[i] = (time_trace[i][1], time_trace[i][0])

  plt.show(block=True)

  trace_string = str(time_trace).replace(',','').replace('[','').replace(']','').replace('(','').replace(')','')

  print(trace_string)


  # sends to a serial port but currently does not work
  send_to_serial(time_trace)

  return time_trace

if __name__ == "__main__":
    main()
