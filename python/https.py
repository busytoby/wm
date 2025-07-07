import http.server
import ssl
import sys
from io import BytesIO
from urllib.parse import urlparse

class hrHandler(http.server.BaseHTTPRequestHandler):
    def _hrHeader(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Dysnomia-version", "0.01")

    def do_GET(self):
        self._hrHeader();
        self.end_headers()
        parsed = urlparse(self.path) 
        self.wfile.write(b"<pre>" + str.encode(str(parsed)) + b"</pre>")

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_body = self.rfile.read(content_length)
        self._hrHeader()
        response = BytesIO()
        response.write(b"<h1>Hello from a POST request!</h1>")
        response.write(b"<p>Received data: </p>")
        response.write(post_body)
        self.wfile.write(response.getvalue())

# Define the server address and port
HOST = "0.0.0.0"
PORT = 53357

# Create an HTTPS server
with http.server.HTTPServer((HOST, PORT), hrHandler) as httpd:
    ctx = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
    ctx.load_cert_chain(certfile='/home/mariarahel/src/wm/python/dystest.pem')
    httpd.socket = ctx.wrap_socket(httpd.socket, server_side=True)
    print(f"Serving HTTPS on {HOST}:{PORT}", file=sys.stderr)
    httpd.serve_forever()
