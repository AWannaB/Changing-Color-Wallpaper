import tkinter as tk
from pynput import mouse
import random
import math


def color_change():
    color = ranHexColor()
    root.configure(bg=color)


# Generates a random time interval of between 0 and 10 seconds in milliseconds
def runTime():
    ms = random.randint(1, 10000)
    return ms


# Generates a random hex color code
def ranHexColor():
    r = random.randint(0, 255)
    g = random.randint(0, 255)
    b = random.randint(0, 255)
    return f'#{r:02x}{g:02x}{b:02x}'


# Changes the color when the mouse is scrolled
def on_scroll(x, y, dx, dy):
    color_change()


# Changes the color if the mouse ever moves more than 500 pixels
def on_move(x, y):
    global last_position
    distance = math.sqrt((x - last_position[0])**2 + (y - last_position[1])**2)
    if distance > 500:
        color_change()
        last_position = (x, y)


# Stops listener and closes the Tkinter window
def close_window(event=None):
    listener.stop()
    root.destroy()


# Changes color at a random but regular interval
def change_color():
    color_change()
    root.after(runTime(), change_color)


# Changes color when key is pressed
def on_key_press(event):
    color_change()


# Changes color when button is clicked
def button_click(event):
    color_change()


# Bind mice buttons presses to the color change function.
def bind_button(*buttons):
    for num in buttons:
        root.bind(f'<Button-{num}>', button_click)


# Create the main window
root = tk.Tk()
# Start with a random color
root.configure(bg=ranHexColor())
# Hide the mouse cursor
root.configure(cursor='none')
#  Make the window Fullscreen
root.attributes('-fullscreen', True)
# Bind the ESC key stop the program
root.bind('<Escape>', close_window)
# Bind any other key to change the color
root.bind('<Key>', on_key_press)
# Bink all mice buttons to chang the color (change to however many buttons your mouse has)
bind_button(1, 2, 3, 4, 5)

# Initialize the last position of the mouse cursor
mouseController = mouse.Controller
initialPos = mouseController().position
last_position = initialPos

# Set up the mouse listener in a separate thread
listener = mouse.Listener(on_scroll=on_scroll, on_move=on_move)
listener.start()
# Set up the regular color changer
change_color()

# Start the Tkinter event loop
root.mainloop()
