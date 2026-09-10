from tkinter import*
from subprocess import*
from time import*

def compress():
    cmd = ['./main.exe', '-c', '-i', infile.get(), '-o', outfile.get()]
    Popen(cmd)
    done.config(text = "Compress Done!!")
    win.after(2000, empty_done)
    

def decompress():
    cmd = ['./main.exe', '-u', '-i', infile.get(), '-o', outfile.get()]
    Popen(cmd)
    done.config(text = "Decompress Done!!")
    win.after(2000, empty_done)

def Compile():
    cmd = [compile_en.get(), 'main.cpp', '-o', './main.exe']
    Popen(cmd)
    done.config(text = "Compile Done!!")
    win.after(2000, empty_done)

def empty_done():
    done.config(text = "")

#Window
win = Tk()
win.geometry("500x400")
win.resizable(False, False)
win.title("Compression and Decompression")

#Title
tit = Label(win, text = "Compresstion and Decompression\n", font = ('Arial', 20, 'bold'))
tit.pack()

#Complie
complie_lb = Label(text = "C++ Complier (e.g. g++)")
complie_lb.config(font = ('Arial', 15))
complie_lb.pack()

compile_en = Entry()
compile_en.pack()

compile_btn = Button(text = "Compile", command = Compile)
compile_btn.pack()

#Space
tmp = Label(text ="\n", font = ('', 10))
tmp.pack(padx = 10)

#Input
infile_lb = Label(text = "Input File")
infile_lb.config(font = ('Arial', 15))
infile_lb.pack()

infile = Entry()
infile.pack()

#Space
tmp = Label(text ="\n", font = ('', 10))
tmp.pack(padx = 10)

#Output
outfile_lb = Label(text = "Output File")
outfile_lb.config(font = ('Arial', 15))
outfile_lb.pack()

outfile = Entry()
outfile.pack()

#Buttom
c = Button(text = "Compression", command = compress)
c.pack()

u = Button(text = "Decompression", command = decompress)
u.pack()

#Done
done = Label(text = "", font = ('Courier', 15), fg = "limegreen")
done.pack()
empty_done()

win.mainloop()