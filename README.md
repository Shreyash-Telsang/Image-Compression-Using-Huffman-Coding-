#BMP Image Compression and Decompression Using Huffman Coding in C#

This project implements a BMP image compression and decompression system using Huffman coding, developed in C. The system is designed to efficiently reduce the size of BMP image files without any loss of information by encoding the image data into a compressed format using Huffman coding and subsequently decoding it back to its original form. The project includes two primary components: a compressor and a decompressor.

1. Compressor Module
The compressor module is responsible for reading a BMP image file, analyzing the pixel data, generating Huffman codes, and then encoding the image into a compressed file format. Below is a detailed breakdown of its functionality:

Reading the BMP Image:

The BMP image file is opened and read into memory. The file's header information is extracted, which includes details such as the image's width, height, and bits per pixel. The pixel data is then loaded into memory for further processing.
Generating Histogram and Frequency Analysis:

A histogram of pixel values is created by scanning through the entire image. This histogram is used to determine the frequency of each unique pixel value in the image. The frequency distribution is crucial for the next step, where Huffman codes are generated.
Building the Huffman Tree:

A Huffman tree is constructed based on the frequency analysis. Each unique pixel value is represented as a leaf node in the tree, with the frequency of the pixel value determining its position in the tree. The Huffman tree is built in a way that the most frequent pixel values are closer to the root, resulting in shorter binary codes.
Generating Huffman Codes:

Once the Huffman tree is built, the compressor generates a set of variable-length binary codes for each unique pixel value. These Huffman codes replace the original pixel values in the image data, significantly reducing the file size.
Encoding the Image:

The pixel data is then encoded using the generated Huffman codes. Each pixel value in the image is replaced by its corresponding Huffman code, and the resulting binary data is written to a new compressed file. Additional information, such as the Huffman code value pairs and any padding bits, is also stored in the compressed file to facilitate accurate decompression later.
Output:

The compressor outputs a compressed file with a .compressed extension. It also provides details about the size of the original BMP file and the compressed file, demonstrating the effectiveness of the compression process.
2. Decompressor Module
The decompressor module is responsible for decoding the compressed image file back into its original BMP format. It reverses the compression process by reading the encoded data, reconstructing the Huffman codes, and restoring the original pixel values. Here's how it works:

Reading the Compressed File:

The decompressor opens the compressed file and reads the BMP header, the Huffman code value pairs, and any padding information. This information is used to accurately decode the compressed data.
Reconstructing Huffman Codes:

Using the Huffman code value pairs read from the compressed file, the decompressor reconstructs the Huffman codes. These codes are essential for mapping the compressed binary data back to the original pixel values.
Decoding the Image:

The decompressor reads the compressed binary data byte by byte, converting it back into the original pixel values using the Huffman codes. The decoded pixel values are then written to a new BMP file, which is an exact replica of the original image before compression.
Output:

The decompressor generates a BMP image file as output. This file is identical to the original BMP image before compression, demonstrating the lossless nature of the Huffman coding compression technique.
Key Functions and Structures
BMPHeader:

A structure that represents the BMP file header and DIB (Device Independent Bitmap) header, storing essential metadata about the image file.
huffNode and minHeap:

Data structures used for building the Huffman tree. The huffNode represents each node in the tree, while the minHeap is used to efficiently build the tree based on the frequency of pixel values.
codeValuePair:

A structure that stores the Huffman code and its corresponding pixel value. This is essential for both encoding and decoding processes.
intToBinary, intToBitArray, binaryArrayToInt:

Utility functions used for converting integers to binary and vice versa, as well as managing the bitwise operations required during the encoding and decoding processes.
Project Structure
Main Files:

compress.c: Contains the code for reading the BMP image, generating Huffman codes, and compressing the image.
decompress.c: Contains the code for reading the compressed file, reconstructing the original image using Huffman codes, and outputting the BMP image.
Headers:

queuell.h: Defines the queue structure and operations used for managing the encoding and decoding processes.
How to Run
Compression:

Compile the compress.c file using a C compiler and run the executable with the BMP image file as an argument. The compressor will generate a compressed file with the .compressed extension.
Decompression:

Compile the decompress.c file and run the executable with the compressed file as an argument. The decompressor will generate a BMP image file as output.
Conclusion
This project demonstrates the use of Huffman coding for efficient image compression, providing a hands-on implementation in C. The project is designed to be extendable, with the potential for further optimization and enhancement, such as supporting different image formats or improving compression ratios. The code is well-documented and structured, making it an excellent starting point for those interested in learning about data compression techniques and their practical applications.
