#!/usr/bin/env python

from argparse import ArgumentParser
import os
import cv2
import apriltag
import numpy
import math
from copy import copy, deepcopy

################################################################################
def apriltag_video(input_streams=['../media/input/single_tag.mp4', '../media/input/multiple_tags.mp4'], # For default cam use -> [0]
                   output_stream=False,
                   display_stream=True,
                   detection_window_name='AprilTag',
                  ):

    '''
    Detect AprilTags from video stream.

    Args:   input_streams [list(int/str)]: Camera index or movie name to run detection algorithm on
            output_stream [bool]: Boolean flag to save/not stream annotated with detections
            display_stream [bool]: Boolean flag to display/not stream annotated with detections
            detection_window_name [str]: Title of displayed (output) tag detection window
    '''

    parser = ArgumentParser(description='Detect AprilTags from video stream.')
    apriltag.add_arguments(parser)
    options = parser.parse_args()

    '''
    Set up a reasonable search path for the apriltag DLL.
    Either install the DLL in the appropriate system-wide
    location, or specify your own search paths as needed.
    '''


    detector = apriltag.Detector(options, searchpath=apriltag._get_dll_path())

    for stream in input_streams:

        video = cv2.VideoCapture(0)

        output = None

        if output_stream:
            width = int(video.get(cv2.CAP_PROP_FRAME_WIDTH))
            height = int(video.get(cv2.CAP_PROP_FRAME_HEIGHT))
            fps = int(video.get(cv2.CAP_PROP_FPS))
            codec = cv2.VideoWriter_fourcc(*'XVID')
            if type(stream) != int:
                output_path = '../media/output/'+str(os.path.split(stream)[1])
                output_path = output_path.replace(str(os.path.splitext(stream)[1]), '.avi')
            else:
                output_path = '../media/output/'+'camera_'+str(stream)+'.avi'
            output = cv2.VideoWriter(output_path, codec, fps, (width, height))

        while(video.isOpened()):

            success, frame = video.read()
            if not success:
                break
            robotCoordinates, poseMatrix1, poseMatrix2, overlay = apriltag.detect_tags(frame,
                                                   detector,
                                                   camera_params= (1094.285869, 1110.03366, 586.92316, 306.77308),
                                                   tag_size=0.0762,
                                                   vizualization=3,
                                                   verbose=3,
                                                   annotation=True)

            coordinate_array_flag = numpy.any(robotCoordinates)
            pose_matrix_flag = numpy.any(poseMatrix1)
            destCoord = [1000 , 800]

            # only prints if coordinate array is filled (if apriltag is detected)
            if (coordinate_array_flag != False):

                if poseMatrix2[0] >=0:
                    robotAngle =  (numpy.arccos(poseMatrix1[0]) / math.pi) * 180
                else:
                    robotAngle =  (numpy.arccos(poseMatrix1[0]) / math.pi) * -180

                #destination coordinates
                destCoordTransposed = [(destCoord[0] - robotCoordinates), (destCoord[1] - robotCoordinates)]

                transposedRobotCoordinates = robotCoordinates[0] - robotCoordinates[0], robotCoordinates[1] - robotCoordinates[1]
                
                destinationAngleFromXAxis = 180 / math.pi * numpy.arctan(destCoordTransposed[1]/destCoordTransposed[0])[1]

                if (robotAngle > destinationAngleFromXAxis):
                    destinationAngle = robotAngle - destinationAngleFromXAxis
                else:
                    destinationAngle = destinationAngleFromXAxis - robotAngle

                print("\r robot position: ",robotCoordinates)
                print("\r robot angle: ", robotAngle)
                print("\r destination angle: ", destinationAngle)

            # if (pose_matrix_flag != False):
            #     print("\r robot angle: %d degrees", robotAngle, end='')


              
            if output_stream:
                output.write(overlay)

            yErrorResolution = 1100

            if display_stream:
                overlay = cv2.flip(overlay, 0)
                overlay = cv2.circle(overlay, (destCoord[0], yErrorResolution - destCoord[1]), radius=15, color=(0, 0, 255), thickness=-1)
                cv2.imshow(detection_window_name, overlay)
                
                if cv2.waitKey(1) & 0xFF == ord(' '): # Press space bar to terminate
                    break

################################################################################

if __name__ == '__main__':
    apriltag_video()