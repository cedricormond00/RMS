import subprocess
import time
import os

# Get the current epoch time
current_epoch = int(time.time())

# Convert the epoch time to a string
epoch_str = str(current_epoch)

# Build and upload the Arduino sketch if it hasn't been done before
sketch_path = "lib/RTC/src/RTC.cpp"
if not os.path.isfile(sketch_path):
    subprocess.run(["platformio", "run", "-t", "upload"])

# Replace the placeholder in the sketch with the current epoch time
with open(sketch_path, "r+") as sketch_file:
    sketch_content = sketch_file.read()
    updated_sketch = sketch_content.replace("value to replace", epoch_str)
    sketch_file.seek(0)
    sketch_file.write(updated_sketch)
    sketch_file.truncate()
