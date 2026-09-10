# Huffman Compressor

使用 C++ 實作 Huffman 檔案壓縮與解壓縮，並提供圖形化介面。

## 執行方式

需要支援 C++11 以上的 C++ 編譯器（例如 `g++`）。請在專案目錄開啟終端機，先編譯：

```sh
g++ -std=c++11 main.cpp -o main.exe
```

壓縮檔案：

```sh
./main.exe -c -i input.txt -o compressed.huf
```

解壓縮檔案：

```sh
./main.exe -u -i compressed.huf -o restored.txt
```

將範例檔名替換為自己的路徑，並依照上述順序提供參數；`-i` 為輸入檔案，`-o` 為輸出檔案。

## 圖形介面（選用）

需要 Python 3 與 Tkinter。完成上面的編譯後，在專案目錄執行：

```sh
python3 gui.py
```

填入**編譯器名稱**、**Input File** 和 **Output File**，按 **Compression** 壓縮或 **Decompression** 解壓縮。