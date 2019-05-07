import socketserver
#import serial
import os

class MyTCPHandler(socketserver.BaseRequestHandler):
    """
    The request handler class for our server.
    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        # self.request is the TCP socket connected to the client
        self.data = self.request.recv(1024).strip()
        
        #continuous saving the data in the txt file (everytime line 18 is excecuted the program emptry out the txt file)
        
        #windows
        file =  open(os.environ['HOMEPATH']+"/range Test 1m 2000m.txt", "ab")
        
        file.write((self.data))
        file.write(b'\n')
        file.close()
        #print("{} wrote:".format(self.client_address[0]))
        print(str(self.data))

if __name__ == "__main__":
    HOST, PORT = "192.168.137.1", 6969
    print("Starting server......")
    # Create the server, binding to localhost on port 9999
    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)
 
    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()
    print("forever")
    