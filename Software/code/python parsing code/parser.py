import serial
import time

def parse_point(line):
    try:
        if line.startswith("X:"):
            parts = line.strip().split()
            x = float(parts[1])
            y = float(parts[3])
            z = float(parts[5])
            return (x, y, z)
    except:
        return None
    return None

def main():
    port = input("Enter COM port (e.g., COM3): ").strip()
    baud_rate = 115200
    ser = serial.Serial(port, baud_rate, timeout=1)
    print("Listening for 3D points on", port)

    points = []

    try:
        while True:
            line = ser.readline().decode(errors='ignore').strip()
            if line == "":
                continue
            point = parse_point(line)
            if point:
                points.append(point)
                print("Point:", point)
            if "Scan completed" in line:
                break

    except KeyboardInterrupt:
        print("\nInterrupted by user.")
    finally:
        ser.close()

    with open("scan_output.ply", "w") as f:
        f.write("ply\nformat ascii 1.0\n")
        f.write(f"element vertex {len(points)}\n")
        f.write("property float x\nproperty float y\nproperty float z\nend_header\n")
        for p in points:
            f.write(f"{p[0]} {p[1]} {p[2]}\n")

    print(f"\nSaved {len(points)} points to 'scan_output.ply'.")

if __name__ == "__main__":
    main()
