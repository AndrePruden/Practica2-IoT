import socket
import threading

# Configuración del servidor
HOST = ''  # Escuchar en todas las interfaces
PORT = 10000

# Crear el socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(2)

print(f'Servidor multihilo escuchando en el puerto {PORT}...')

# Lista para almacenar las conexiones de los clientes
clients = []

def handle_client(conn, addr):
    print(f'Conexión establecida con {addr}')
    conn.settimeout(120)
    
    try:
        while True:
            data = conn.recv(1024).decode().strip()
            if not data:
                print(f'Cliente {addr} desconectado')
                break

            print(f'Datos recibidos de {addr}: {data}')

            if "DISTANCE:" in data:
                continue

            try:
                distance = float(data)
                print(f'Distancia válida recibida de {addr}: {distance}')
                
                if 5 <= distance <= 10:
                    broadcast(b'STATE 3\n')
                elif 15 <= distance <= 20:
                    broadcast(b'STATE 2\n')
                elif 25 <= distance <= 30:
                    broadcast(b'STATE 1\n')
                else:
                    broadcast(b'STATE 0\n')

            except ValueError:
                print("Error: Distancia no es un número válido")

    except ConnectionResetError:
        print(f'Conexión con {addr} cerrada de manera abrupta')
        # Enviar mensaje de reinicio a todos los clientes
        broadcast(b'RESTART\n')

    finally:
        conn.close()
        clients.remove(conn)
        print(f'Conexión cerrada con {addr}')

def broadcast(message):
    """Envía el mensaje a todos los clientes conectados (uC B)"""
    for client in clients:
        try:
            client.sendall(message)
        except:
            print("Error enviando mensaje a un cliente")

def reset_microcontrollers():
    """Envía un comando de reinicio a los microcontroladores"""
    reset_command = b'RESET\n'  # Comando para reiniciar los microcontroladores
    broadcast(reset_command)
    print("Comando de reinicio enviado a los microcontroladores.")

def accept_connections():
    """Función para aceptar conexiones de múltiples clientes"""
    while True:
        conn, addr = server_socket.accept()
        clients.append(conn)
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()

# Iniciar la función que acepta conexiones
accept_connections()
