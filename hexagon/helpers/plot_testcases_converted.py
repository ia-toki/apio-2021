#!/usr/bin/env python3

import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import os
import shutil
import sys

def GeneratePlotFromFile(tc_dir, image_dir, tc):
  plt.close()

  f = open(os.path.join(tc_dir, tc + ".in"), "r")
  key = f.readline().strip()
  n, a, b = map(int, f.readline().strip().split())
  tc_path = os.path.join(tc_dir, tc + ".in")
  print(f'Generating plot for {tc_path}: N = {n}', end='', flush=True)

  pdirs = [(0, 1), (1, 1), (1, 0), (0, -1), (-1, -1), (-1, 0)]

  now = (0, 0)
  points = [now]
  for _ in range(n):
    d, l = map(int , f.readline().strip().split())
    d -= 1
    now = (now[0] + pdirs[d][0] * l, now[1] + pdirs[d][1] * l)
    points.append(now)

  p = Polygon(points, True)
  ax = plt.gca()
  ax.add_patch(p)
  ax.autoscale()
  plt.savefig(os.path.join(image_dir, tc + ".png"))
  print('\t[Done]')


def GeneratePlots(tc_dir, image_dir):

  for root, _, files in os.walk(tc_dir):
    for filename in files:
      if len(filename) < 3 or filename[-3:] != ".in":
        continue
      tc = filename[:-3]
      GeneratePlotFromFile(tc_dir, image_dir, tc)

def main():
  if len(sys.argv) < 3:
    print("Usage: {} <image_dir> <tc_dir>...".format(sys.argv[0]))
    print("       e.g. {} tests/plots tests".format(sys.argv[0]))
    return

  image_dir = sys.argv[1]
  print('Generating plot to folder {}...'.format(image_dir))


  if os.path.exists(image_dir):
    shutil.rmtree(image_dir)
  os.makedirs(image_dir)

  for i in range(2, len(sys.argv)):
    GeneratePlots(sys.argv[i], image_dir)

if __name__ == "__main__":
  main()
