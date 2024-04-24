import time
import math
import sys
import queue
import multiprocessing
import threading
from datetime import datetime
from sort_utils import bubblesort

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
    for number in sorted_chunks[0]:
        outfile.write(str(number) + "\n")
    total_time = ( endTime - starttime )
    current_time = datetime.now()
    outfile.write( "CPU Time : " + str(total_time)  + " seconds \n" )
    outfile.write( "Output Time : " + str(current_time)  + "+08:00\n" ) 
    outfile.close() 

def merge(arr, l, mid, r):
    n1 = mid - l + 1
    n2 = r - mid

    L = arr[l:l+n1]
    R = arr[mid+1:mid+1+n2]

    i = j = 0
    k = l

    while i < n1 and j < n2:
        if L[i] <= R[j]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        k += 1

    while i < n1:
        arr[k] = L[i]
        i += 1
        k += 1

    while j < n2:
        arr[k] = R[j]
        j += 1
        k += 1

def M4merge_sort(arr, l, r):
    if l < r:
        mid = (l + r) // 2
        M4merge_sort(arr, l, mid)
        M4merge_sort(arr, mid+1, r)
        merge(arr, l, mid, r)

def Method4(arr,filename):
    output = ""
    output = filename
    k = int(input("Please Input K\n"))
    n = len(arr)
    chunk_size = len(arr) // k
    starttime = time.time()

    n = len(arr)
    chunk_size = n // k

    # Split array into chunks
    chunks = [arr[i:i+chunk_size] for i in range(0, n, chunk_size)]

    # Bubble sort each chunk using threading
    bubble_threads = []
    for chunk in chunks:
        thread = threading.Thread(target=bubblesort, args=(chunk,))
        bubble_threads.append(thread)
        thread.start()

    for thread in bubble_threads:
        thread.join()

    # Merge all sorted chunks using threading
    while len(chunks) > 1:
        merged_chunks = []
        for i in range(0, len(chunks), 2):
            if i+1 < len(chunks):
                merged = chunks[i] + chunks[i+1]
                M4merge_sort(merged, 0, len(merged)-1)
                merged_chunks.append(merged)
            else:
                merged_chunks.append(chunks[i])
        chunks = merged_chunks


    endTime = time.time()
    outfile = open(output + "_output4.txt", "w")
    outfile.write("Sort : \n")
    for i in range ( len(chunks) ):
        outfile.write(str(chunks[i]) + "\n")
    total_time = ( endTime - starttime )
    current_time = datetime.now()
    outfile.write( "CPU Time : " + str(total_time)  + " seconds \n" )
    outfile.write( "Output Time : " + str(current_time.isoformat()) + "+08:00\n" )
    outfile.close()    

if __name__ == '__main__':
    command = 9
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
                pass
        
        elif command == "2" :
            filename = input("Please input  file name.\n") 
            if Inputtxt(arr, filename) == True:
                Method2(arr,filename)
            else:
                pass
            
        elif command == "3" :
            filename = input("Please input  file name.\n") 
            if Inputtxt(arr, filename) == True:
                Method3(arr,filename)
            else:
                pass
            
        elif command == "4" :
            filename = input("Please input  file name.\n") 
            if Inputtxt(arr, filename) == True:
                Method4(arr,filename)
            else:
                pass
        
        else :
            print("No such command. Please try again.")
