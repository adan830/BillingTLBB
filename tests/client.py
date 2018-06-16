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

messageHex = 'AA55008EA20002096164666164666164662065336365623538383161306131666461616430313239366437353534383638640D3139322E3136382E302E313732000130303030303000000000000030356634646166643866663430613662633639306231643262373562653965362338304634314242322D44354138334235332D33323441424141342D364433363742384455AA'

host = socket.gethostname()
port = 12680

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
  s.connect((host, port))
except:
  print('Error')
else:
  while messageHex != 'q':
    s.sendall(binascii.unhexlify(messageHex))

    data = s.recv(1024)

    print('Received', repr(data))
    print('In Hex: {}', byteToHex(data))

    messageHex = raw_input('Insert next Hex Data: ')

s.close()

