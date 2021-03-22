#include <string>
#include <fstream>  
#include <iostream>
#include <cstdlib> 
#include <ctime>

using namespace std;
 
/**
 * @brief Функция для записи массива в файл.
 * 
 * @param filename Имя файла.
 * @param arr Указатель на массив.
 * @param n Размер массива.
 */
void write_arr(const string& filename, const int* arr, const int n)
{
    fstream fs;

    fs.open(filename, fstream::out);
    if (fs.is_open()) 
    {
        for (int i = 1; i < n; i++)
        {
            fs << arr[i - 1] << ' '; // записываем значения через пробел
        }
        fs << arr[n - 1];

        fs.close();
    }
}

/**
 * @brief Функция для смены местами двух значений.
 *  
 * @param a Певое значение.
 * @param b Второе значение.
 */
void swap(int* a, int* b) // 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* arr, int low, int high)
{
    int pivot = arr[high];    // опорный элемент
    int i = (low - 1);  // индекс наименьшего элемента
    for (int j = low; j <= high - 1; j++)
    {
        // если текущий элемент меньше или равен опорнуму
        if (arr[j] <= pivot)
        {
            i++;    // увеличиваем индекс минимального элемента
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/**
 * @brief Функций быстрой сортировки массива.
 * 
 * @param arr Масссива для сортировки.
 * @param low Элемент начала сортировки.
 * @param high Элемент конца сортировки.
 */
void quickSort(int* arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high); // индекс опорного элемента
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/**
 * @brief Функция копирования файлов.
 * 
 * @param fileSrcName Исходный файл.
 * @param fileDstName Файл назначения.
 */
void fileCopy(const string& fileSrcName, const string& fileDstName)
{
    ifstream src;
    ofstream dst;

    src.open(fileSrcName, ios::in | ios::binary);
    dst.open(fileDstName, ios::out | ios::binary);

    if (src.is_open() && dst.is_open())
    {
        dst << src.rdbuf();
        src.close();
        dst.close();
    } 
}

void merge2Files(const string& file1Name, const string& file2Name, const string& fileOutName) // тут принимать значения по ссылкам 
{
    fstream fileIn1;
    fstream fileIn2;
    fstream fileOut;

    fileIn1.open(file1Name, fstream::in);
    fileIn2.open(file2Name, fstream::in);
    fileOut.open(fileOutName, fstream::out);

    int number1 = 0, number2 = 0; 
    if (fileIn1.is_open() && fileIn2.is_open() && fileOut.is_open())
    {
        fileIn1 >> number1; 
        fileIn2 >> number2;
        while (!fileIn1.fail() && !fileIn2.fail())
        {
            if (number1 < number2)
            {
                fileOut << number1 << ' ';
                fileIn1 >> number1;
            }
            else
            {
                fileOut << number2 << ' ';
                fileIn2 >> number2;
            }
        }

        while (!fileIn1.fail())
        {
            fileOut << number1 << ' ';
            fileIn1 >> number1;
        }
        while (!fileIn2.fail())
        {
            fileOut << number2 << ' ';
            fileIn2 >> number2;
        }

        fileIn1.close();
        fileIn2.close();
        fileOut.close();

        remove(file1Name.c_str());
        remove(file2Name.c_str());
    }
}

/**
 * @brief Функция для соединения и сортировки данных файлов с шаблонным именем. 
 * 
 * @param fileNameTmpl Шаблой именования файлов. Например если параметр "array_data" то будут соеденены и отсортированны все файлы с название array_dataN.txt где N - номер файла и меньше fileCount.
 * @param fileCount Колличество файлов которые даныне которых требуется отсортировать и соеденить в один файл. Функция задействует все файлы от 0 до fileCount.
 */
void mergeFiles(const string& fileNameTmpl, unsigned int fileCount)
{
    int nextFileNumber = 0;

    if (fileCount <= 1)
    {
        char fileIn1Name[1000];
        sprintf_s(fileIn1Name, "%s%d.txt", fileNameTmpl.c_str(), 0); // тут везде сделать string

        char fileOutName[1000];
        sprintf_s(fileOutName, "%s_out.txt", fileNameTmpl.c_str());

        fileCopy(fileIn1Name, fileOutName);

        remove(fileIn1Name);

        return;
    }

    // Учитываем случай когда файлов нечетное кол-во
    if (fileCount % 2)
    {
        char fileIn1Name[1000];
        sprintf_s(fileIn1Name, "%s%d.txt", fileNameTmpl.c_str(), fileCount - 2); // тут везде сделать string

        char fileIn2Name[1000];
        sprintf_s(fileIn2Name, "%s%d.txt", fileNameTmpl.c_str(), fileCount - 1); // тут везде сделать string

        char fileBufName[1000];
        sprintf_s(fileBufName, "buf.txt"); // тут везде сделать string

        merge2Files(fileIn1Name, fileIn2Name, fileBufName);

        fileCopy(fileBufName, fileIn1Name);

        remove(fileBufName);
        remove(fileIn2Name);
    }
    
    for (int i = 0; i < fileCount - fileCount % 2; i += 2)
    {
        char fileIn1Name[1000];
        sprintf_s(fileIn1Name, "%s%d.txt", fileNameTmpl.c_str(), i); // тут везде сделать string

        char fileIn2Name[1000];
        sprintf_s(fileIn2Name, "%s%d.txt", fileNameTmpl.c_str(), i + 1); // тут везде сделать string

        char fileBufName[1000];
        sprintf_s(fileBufName, "buf.txt"); // тут везде сделать string

        merge2Files(fileIn1Name, fileIn2Name, fileBufName);

        char fileOutName[1000];
        sprintf_s(fileOutName, "%s%d.txt", fileNameTmpl.c_str(), nextFileNumber++);
        fileCopy(fileBufName, fileOutName);

        remove(fileBufName);
    } 

    int newFileCout = fileCount / 2;

    if (newFileCout > 1) // Если файлы еще остались то рекурсивно вызываем функцию
    {
        mergeFiles("array_data", newFileCout); // тут заменить имя на fileNameTmpl
    }
    else
    {
        char fileLastName[1000];
        sprintf_s(fileLastName, "%s%d.txt", fileNameTmpl.c_str(), 0);

        char fileEndName[1000];
        sprintf_s(fileEndName, "%s_out.txt", fileNameTmpl.c_str());

        fileCopy(fileLastName, fileEndName);

        remove(fileLastName);
    }
}

/**
 * @brief Функция генерации файла данных, для дальнейшей сортировки. 
 * 
 * @param filename Имя файла генерации.
 * @param dataFileSize Размер итогового файла.Даже если параметр == 0, файл будет с одним числом int.
 */
void fileGenerate(string& filename, const unsigned int dataFileSize)
{
    cout << "Generating data file..." << endl;
    fstream fs;
    fs.open(filename, fstream::out);
    srand(time(0));
    int size = 0;
    while (size < dataFileSize)
    {
        fs << (rand() % 1000) << ' ';
        // fs.seekg(0, std::ios::end);
        size = fs.tellg();
    }
    fs << (rand() % 1000);
    cout << "File is generated." << endl;
    cout << "Size of file : " << size + sizeof(int) << " B." << endl;
    fs.close();
}

/**
 * @brief Программа сортировки чисел в больших файлах.
 * Алгоритм:
 * 1) С файла считывается массив гарантированно помещающийся в память.
 * 2) Массив сортируется быстрой сортировкой и помещается в отдельный файл.
 * 3) П 1 и 2 повторяются пока исходный файл не закончится.
 * 4) Файлы с отсортированными массивами попарно соединяются в один файл, при соединении просиходит сортировка данных файлов. 
 */
int main()
{
    string filename = "array_data.txt";
    const unsigned int dataFileSize = 10485760; // 10 MB 
    size_t arrSize = 262144; // задаем размер массива размещаемого в памяти, при int == 4 байт массив занимает 1 Мб.

    fileGenerate(filename, dataFileSize);
    
    fstream fs;
    fs.open(filename, fstream::in);
    if (fs.is_open())
    {
        cout << "Splitting file and sorted..." << endl;
        int fileCount = 0;
        while (!fs.eof())
        {
            int* arr = new int[arrSize];
            unsigned int realArrSize = 0;
            for (int i = 0; i < arrSize && !fs.eof(); ++i)
            {
                fs >> arr[i]; // читаем из файла разделительные символы - пробел и перенос строки
                ++realArrSize;
            }
            
            quickSort(arr, 0, realArrSize - 1);
            
            char outFileName[1000];
            sprintf_s(outFileName, "array_data%d.txt", fileCount++);
            write_arr(outFileName, arr, realArrSize);
            delete[] arr;
        }
        cout << "Merge files..." << endl;
        mergeFiles("array_data", fileCount);
        fs.close();
        cout << "Completed" << endl;
    }
   
    return 0;
}
