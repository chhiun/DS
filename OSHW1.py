import time
import math
import sys
import queue
import multiprocessing
import threading
from datetime import datetime
"""method3 -> pool, method4-> threads"""
def Inputtxt(arr, filename):
    while(True):
        try:
            inputfile = open(filename + ".txt","r")      
            break
        except FileNotFoundError:
            print("No such file.")
            return False
            
    
    s = inputfile.readline()
    s = s[0:len(s)-1]
    
    while s.isdigit() :
        arr.append( int(s) )
        s = inputfile.readline()
        s = s[0:len(s)-1]
        
    inputfile.close() 
    return True
    

def bubblesort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n-i-1):
            if arr[j] > arr[j+1]:
                arr[j], arr[j+1] = arr[j+1], arr[j]   
    return arr

def Method1(arr,filename):
    output = ""
    output = filename
    starttime = time.time()
    bubblesort(arr)
    endTime = time.time()
    outfile = open(output + "_output1.txt", "w")
    outfile.write("Sort : \n")
    for i in range ( len(arr) ):
        outfile.write(str(arr[i]) + "\n")  
    total_time = ( endTime - starttime )
    current_time = datetime.now()
    outfile.write( "CPU Time : " + str(total_time)  + " seconds \n" )
    outfile.write( "Output Time : " + str(current_time)  + "+08:00\n" )
    outfile.close()    


def Method2(arr,filename):
    output = ""
    output = filename
    k = int(input("Please Input K\n"))
    chunk_size = len(arr) // k
    starttime = time.time()

    # Bubble sort each chunk
    for i in range(0, len(arr), chunk_size):
        chunk = arr[i:i+chunk_size]
        bubblesort(chunk)
        arr[i:i+chunk_size] = chunk

    # Merge all sorted chunks
    merge_sort(arr)

    endTime = time.time()
    outfile = open(output + "_output2.txt", "w")
    outfile.write("Sort : \n")
    for i in range ( len(arr) ):
        outfile.write(str(arr[i]) + "\n")
    
    total_time = ( endTime - starttime )
    current_time = datetime.now()
    outfile.write( "CPU Time : " + str(total_time)  + " seconds \n" )
    outfile.write( "Output Time : " + str(current_time)  + "+08:00\n" )
    outfile.close()  

def merge_sort(arr):
    if len(arr) > 1:
        mid = len(arr) // 2
        L = arr[:mid]
        R = arr[mid:]

        merge_sort(L)
        merge_sort(R)

        i = j = k = 0

        while i < len(L) and j < len(R):
            if L[i] < R[j]:
                arr[k] = L[i]
                i += 1
            else:
                arr[k] = R[j]
                j += 1
            k += 1

        while i < len(L):
            arr[k] = L[i]
            i += 1
            k += 1

        while j < len(R):
            arr[k] = R[j]
            j += 1
            k += 1

def Method3(arr,filename):
    output = ""
    output = filename
    k = int(input("Please Input K\n"))
    n = len(arr)
    chunk_size = len(arr) // k
    starttime = time.time()

    # Split array into chunks
    chunks = [arr[i:i+chunk_size] for i in range(0, n, chunk_size)]

    # Bubble sort each chunk using multiprocessing
    with multiprocessing.Pool(processes=k) as pool:
        sorted_chunks = pool.map(bubblesort, chunks)

    # Merge all sorted chunks using multiprocessing
    while len(sorted_chunks) > 1:
        next_chunks = []
        for i in range(0, len(sorted_chunks), 2):
            if i+1 < len(sorted_chunks):
                merged = sorted_chunks[i] + sorted_chunks[i+1]
                merged.sort()
                next_chunks.append(merged)
            else:
                next_chunks.append(sorted_chunks[i])
        sorted_chunks = next_chunks

    endTime = time.time()
    outfile = open(output + "_output3.txt", "w")
    outfile.write("Sort : \n")
    for i in range ( len(sorted_chunks) ):
        outfile.write(str(sorted_chunks[i]) + "\n")
    for num in sorted_chunks:
        print(num)
    total_time = ( endTime - starttime )
    current_time = datetime.now()
    outfile.write( "CPU Time : " + str(total_time)  + " seconds \n" )
    outfile.write( "Output Time : " + str(current_time)  + "+08:00\n" ) 
    outfile.close() 

def Method4(arr,filename):
    output = ""
    output = filename
    k = int(input("Please Input K\n"))
    n = len(arr)
    chunk_size = len(arr) // k
    starttime = time.time()

    # Split array into chunks
    chunks = [arr[i:i+chunk_size] for i in range(0, n, chunk_size)]

    # Bubble sort each chunk using multiprocessing
    with multiprocessing.Pool(processes=k) as pool:
        sorted_chunks = pool.map(bubblesort, chunks)

    # Merge all sorted chunks using multiprocessing
    while len(sorted_chunks) > 1:
        next_chunks = []
        for i in range(0, len(sorted_chunks), 2):
            if i+1 < len(sorted_chunks):
                merged = sorted_chunks[i] + sorted_chunks[i+1]
                merged.sort()
                next_chunks.append(merged)
            else:
                next_chunks.append(sorted_chunks[i])
        sorted_chunks = next_chunks

    endTime = time.time()
    outfile = open(output + "_output3.txt", "w")
    outfile.write("Sort : \n")
    for i in range ( len(sorted_chunks) ):
        outfile.write(str(sorted_chunks[i]) + "\n")
    total_time = ( endTime - starttime )
    current_time = datetime.now()
    outfile.write( "CPU Time : " + str(total_time)  + " seconds \n" )
    outfile.write( "Output Time : " + str(current_time)  + "+08:00\n" ) 
    outfile.close()                      
if __name__ == '__main__':
    command = 9
    sys.setrecursionlimit(1000000)
    while command != 0 :
        print("*************** " )
        print("0 to quit.")
        print("1 to BubbleSort.")
        print("2 to MergeSort.")
        print("3 to use k Process bubble sort and k-1 process merge sort.")
        print("4 to use k Thread bubble sort and k-1 thread merge sort.")
        command = input() 
        filename = ""
        arr = []
        
        if command == "0" :
            break
        
        elif command == "1" :
            filename = input("Please input  file name.\n") 
            if Inputtxt(arr, filename) == True:
                Method1(arr,filename)
            else:
                break
        
        elif command == "2" :
            filename = input("Please input  file name.\n") 
            if Inputtxt(arr, filename) == True:
                Method2(arr,filename)
            else:
                break
            
        elif command == "3" :
            filename = input("Please input  file name.\n") 
            if Inputtxt(arr, filename) == True:
                Method3(arr,filename)
            else:
                break
            
        elif command == "4" :
            break
        
        else :
            print("No such command. Please try again.")





