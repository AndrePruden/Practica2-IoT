import socket
import threading

# Server configuration
HOST = ''  # Listen on all interfaces
PORT = 10000

# Create the socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(2)

print(f'Multi-threaded server listening on port {PORT}...')

# List to store client connections
clients = []

def handle_client(conn, addr):
    print(f'Connection established with {addr}')
    conn.settimeout(120)
    
    try:
        while True:
            data = conn.recv(1024).decode().strip()
            if not data:
                print(f'Client {addr} disconnected')
                break

            print(f'Data received from {addr}: {data}')

            if "DISTANCE:" in data:
                continue

            try:
                distance = float(data)
                print(f'Valid distance received from {addr}: {distance}')
                
                if 5 <= distance <= 10:
                    broadcast(b'STATE 3\n')
                elif 15 <= distance <= 20:
                    broadcast(b'STATE 2\n')
                elif 25 <= distance <= 30:
                    broadcast(b'STATE 1\n')
                else:
                    broadcast(b'STATE 0\n')

            except ValueError:
                print("Error: Distance is not a valid number")

    except ConnectionResetError:
        print(f'Connection with {addr} abruptly closed')
        # Send restart message to all clients
        broadcast(b'RESTART\n')

    finally:
        conn.close()
        clients.remove(conn)
        print(f'Connection closed with {addr}')

def broadcast(message):
    """Sends the message to all connected clients (uC B)"""
    for client in clients:
        try:
            client.sendall(message)
        except:
            print("Error sending message to a client")

def reset_microcontrollers():
    """Sends a reset command to the microcontrollers"""
    reset_command = b'RESET\n'  # Command to reset the microcontrollers
    broadcast(reset_command)
    print("Reset command sent to the microcontrollers.")

def accept_connections():
    """Function to accept multiple client connections"""
    while True:
        conn, addr = server_socket.accept()
        clients.append(conn)
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()

# Start the function that accepts connections
accept_connections()
