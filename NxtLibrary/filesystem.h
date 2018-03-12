#ifndef SYSTEMFILEH
#define SYSTEMFILEH
#include "connection.h"

#ifdef NXTLIBRARY_EXPORTS
#define NXTLIBRARY_API __declspec(dllexport)
#else
#define NXTLIBRARY_API __declspec(dllimport)
#endif

#define MAX_NAME_LENGTH 20
/**
* Enumeration for file modes when opening a file
* @see Filesystem#open
*/
enum NXTLIBRARY_API File_mode{
  /**
  * Open the file for reading
  */
  READ=0x00,

  /**
  * append data to the file (if files does not exist a new file is created)
  */
  APPEND=0x01,

  /**
  * create and write data to a file (before closing the file it must be filled with data otherwise it will be deleted)
  */
  WRITE=0x02,

  /**
  * create and write data to a linear non-fragmented file - should be used with "ric"(picture) and "rxe"(program) files (before closing the file it must be filled with data otherwise it will be deleted)
  */
  WRITE_LINEAR = 0x03,

  /**
  * create and write data to a linear non-fragmented data file (before closing the file it must be filled with data otherwise it will be deleted)
  */
  WRITE_LINEAR_DATA = 0x04

};

/**
* Struct that holds information on a nxt file
* @see Filesystem
*/
struct NXTLIBRARY_API Nxt_file_t{
  /**
  * The name of the file (maximum 19 characters + termination)
  */
  char name[MAX_NAME_LENGTH];

  /**
  * The NXT file handel ID
  */
  unsigned char handle;

  /**
  * The size of the file in bytes
  */
  unsigned int size;
};

/**
* typedef for Nxt_file_t
* @see Nxt_file_t
* @see Filesystem
*/
typedef NXTLIBRARY_API struct Nxt_file_t Nxt_file;


using namespace std;

/**
  * Class to interact with the filesystem on the NXT
*/
class NXTLIBRARY_API Filesystem{
  public:
    /**
     * @param *connection [attach a connection]
     */
    Filesystem(Connection *connection);
    ~Filesystem();

    /**
     * Get remaining flash memory
     * @return the remaining flash memory in bytes
    */
    unsigned int get_free_flash();

    /**
     * Delete the flash memory - all user programs and data
    */
    void delete_flash();

    /**
     * Delete a file on the NXT
     * @param file_name [the name of the file to delete]
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void delete_file(string file_name, bool reply=false);

    /**
     * Delete a file on the NXT
     * @param file [a Nxt_file struct of the file to close]
     * @param reply [true = require reply from NXT; false = no reply from NXT]
    */
    void delete_file(Nxt_file &file, bool reply=false);

    /**
     * Download a file from the NXT to the PC
     * @param pc_name [path to the download destination (example: c:\\download.txt)]
     * @param nxt_name [name of the file on the NXT (example: "program.rxe")]
    */
    void download_file(string pc_name, string nxt_name);

    /**
     * Upload a file from the PC to the NXT
     * @param pc_name [path to the PC file to upload (example: c:\\upload.txt)]
     * @param nxt_name [name of the file on the NXT (example: "data.txt")]
    */
    void upload_file(string pc_file, string nxt_file);

    /**
     * Create a file list
     * @param wild_card [search cretia]
     * <UL>
     * <LI>filename.extension (search for a specific file )
     * <LI>*.[file name type] (search for file type)
     * <LI>filename.* (search for files with a specific filename)
     * <LI>*.* (list all files)
     * </UL>
     * @return number of files in the file list
    */
    unsigned int create_file_list(string wild_card);

    /**
     * Get the number of files in the file list
     * @return number of files in the file list
    */
    unsigned int get_file_list_size();

    /**
     * Get a element from the file list
     * @param idx [the element to return ( 0= first element in the list)]
     * @return a nxt file struct (if idx is out of range all attributes in the struct will be zero
     * - use the file list size to make sure you are not out of range)
     */
    Nxt_file get_file_list_element(unsigned int idx);


    /**
     * Open or create a file on the NXT
     * @param file_name [the name of the file to create or open]
     * @param mode [use the FILE_MODE enumeration how the file should be created or opended]
     * @param file_size [the size of the file in bytes (will be ignored in append and read mode)]
     * @return a nxt file struct containing the data of the openend file
     */
    Nxt_file open_file(string file_name, File_mode mode, unsigned int file_size=0);

    /**
     * read a byte string from a file
     * @param &file [Nxt_file struct of the file to read data from]
     * @param *buffer [a pointer to a buffer that can hold the read bytes]
     * @param no_bytes [bytes to read]
     * @return number of bytes that was read
    */
    unsigned int read_file(Nxt_file &file, unsigned char *buffer, unsigned int num_bytes);

    /**
     * write a byte string to a file
     * @param &file [Nxt_file struct of the file to write data to]
     * @param *buffer [a pointer to a buffer that can hold the bytes to write]
     * @param no_bytes [bytes to write]
     * @return number of bytes that was written
    */
    unsigned int write_file(Nxt_file &file, unsigned char *buffer, unsigned int num_bytes);

    /**
     * close a file on the nxt
     * @param &file [Nxt_file struct of the file to close]
    */
    void close_file(Nxt_file &file);

  private:
    Connection *connection;
    int get_next(int handle, Nxt_file *file);
    int get_first(string wild_card, Nxt_file *file);
    unsigned int update_file_list(string wild_card);
    unsigned int file_list_size;
    Nxt_file *file_list;
    /**
     * To prevent copy - to lazy to create a copy constructor
    */
    Filesystem(const Filesystem& p);//to prevent copy - to lazy to create a copy constructor
    Nxt_file open_read(string file_name);
    Nxt_file open_append(string file_name);
    Nxt_file open_write(string file_name, unsigned int file_size, char type);
    unsigned short int read_command(Nxt_file &file, unsigned char *buffer, unsigned short int num_bytes);
    unsigned short int write_command(Nxt_file &file, unsigned char *buffer, unsigned short int num_bytes);

};




#endif
