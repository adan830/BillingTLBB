import socket
import binascii

def byteToHex( byteStr ):
  """
  Convert a byte string to it's hex string representation e.g. for output.
  """

  # Uses list comprehension which is a fractionally faster implementation than
  # the alternative, more readable, implementation below
  #
  #    hex = []
  #    for aChar in byteStr:
  #        hex.append( "%02X " % ord( aChar ) )
  #
  #    return ''.join( hex ).strip()

  return ''.join( [ "%02X " % ord( x ) for x in byteStr ] ).strip()

messageHex = 'AA550011A0000300000065093132372E302E302E3155AA'

host = socket.gethostname()
port = 12680

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((host, port))

while messageHex != 'q':
  s.sendall(binascii.unhexlify(messageHex))

  data = s.recv(1024)

  print('Received', repr(data))
  print('In Hex: {}', byteToHex(data))

  messageHex = raw_input('Insert next Hex Data: ')

s.close()

