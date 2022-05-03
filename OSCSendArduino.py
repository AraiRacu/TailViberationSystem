import argparse
import math
import random
import time

from pythonosc import dispatcher
from pythonosc import osc_server
from pythonosc import udp_client

_M5Stack_IP = "192.168.XX.XX"
_M5Stack_Port = XX

_VRC_Parametor = "/avatar/parameters/Tail_IsGrabbed"

def Send2Arduino(unused_addr, args, *values):
  if args == 1.0:
    client.send_message("/filter", 1)
  else:
    client.send_message("/filter", 0)
  print(args)

if __name__ == "__main__":
  #M5Stackへ送信
  toM5stack = argparse.ArgumentParser()
  toM5stack.add_argument("--ip", default=_M5Stack_IP,
    help="The ip of the OSC server")
  toM5stack.add_argument("--port", type=int, default=_M5Stack_Port,
    help="The port the OSC server is listening on")
  argsM5 = toM5stack.parse_args()

  client = udp_client.SimpleUDPClient(argsM5.ip, argsM5.port)
  
  #VRCからの受信
  fromVRC = argparse.ArgumentParser()
  fromVRC.add_argument("--ip",
                      default="127.0.0.1", help="The ip to listen on")
  fromVRC.add_argument("--port",
                      type=int, default=9001, help="The port to listen on")
  argsVRC = fromVRC.parse_args()

  #特定のパラメータを受信したら関数に飛ぶ
  dispatcher = dispatcher.Dispatcher()
  dispatcher.map(_VRC_Parametor, Send2Arduino)

  #サーバーの起動
  server = osc_server.ThreadingOSCUDPServer(
      (argsVRC.ip, argsVRC.port), dispatcher)
  print("Serving on {}".format(server.server_address))
  server.serve_forever()