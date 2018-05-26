import serial
port = '/dev/tty.usbmodem1411'
ser = serial.Serial(port, 9600, timeout=0.5)

state = 0

data = []
while True:
    line = ser.readline()

    if state == 0 and line.startswith(b'Starting'):
        state = 1
        data.append({'time':0, 'x': 0, 'y':0, 'z':0})
    elif state == 1 and line.startswith(b'['):
        # Read the data from serial
        linechunks = line[1:-3].split(b',')
        row = {
            'time': float(linechunks[0]),
            'x_acc': float(linechunks[1]),
            'y_acc': float(linechunks[2]),
            'z_acc': float(linechunks[3]),
        }

        # Do the math
        previous_row = data[-1]
        time_elapsed = row['time'] - previous_row['time']
        x_offset = row['x_acc'] * time_elapsed * time_elapsed
        row['x'] = previous_row['x'] + x_offset
        y_offset = row['y_acc'] * time_elapsed * time_elapsed
        row['y'] = previous_row['y'] + y_offset
        z_offset = row['z_acc'] * time_elapsed * time_elapsed
        row['z'] = previous_row['z'] + z_offset

        data.append(row)
        print(row)
