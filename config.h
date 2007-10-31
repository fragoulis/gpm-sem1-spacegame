
#pragma once

#include <string>
#include <fstream>

using std::string;
using std::ifstream;
using std::ios_base;

namespace tlib
{

    /**
     * This class is responsible for reading database values from 
     * a configuration file.
     * The file format supported is of [key=>value] type where
     * a key is a keyword which we search by and value is data of 
     * any type that we want to read.
     * The key and the value are separated by one or more spaces.
     * Each key-value pair is separated by a newline.
     */
    class Config
    {
    private:
        // The file stream
        ifstream cfg;

    public:
        /**
         * Constructors
         */
        Config() {}
        Config( const char *cfg_filename )
        {
            open( cfg_filename );
        }

        /**
         * Opens the stream and returns whether it was successfull
         */
        bool open( const char *cfg_filename )
        {
            cfg.open( cfg_filename );
            return cfg.is_open();
        }

        /**
         * Closes the stream
         */
        void close() { cfg.close(); }

        /**
         * Maps the search to an int or an array of ints
         */
        int getInt( const string &key, int *result, int count = 1 ) {
            return _get<int>( key, result, count );
        }

        /**
         * Maps the search to a float or an array of floats
         */
        int getFloat( const string &key, float *result, int count = 1 ) {
            return _get<float>( key, result, count );
        }

        /**
         * Maps the search to a double or an array of doubles
         */
        int getDouble( const string &key, double *result, int count = 1 ) {
            return _get<double>( key, result, count );
        }

        /**
         * Maps the search to a string
         */
        int getString( const string &key, char result[] ) {
            return _get<char*>( key, &result, 1 );
        }

        /**
         * Maps the search to a string
         */
        int getString( const string &key, string *result ) {
            return _get<string>( key, result, 1 );
        }

    private:
        /**
         * Searches the configuration file to find the desired key-value pair
         * we want.
         * Returns -1 if file is not open, 0 if key was not found and 1 if
         * key was found and value was saved successfully
         */
        template<typename T>
        int _get( const string &key, T *result, const int count )
        {
            if( !cfg.is_open() ) return -1;

            // For every line in the file, read the first word
            // (aka. stop at the first space character)
            char line[50];
            int sz = sizeof(line);
            while( cfg.getline( line, sz, ' ' ) )
            {
                // Compare word with given key
                if( 0 == key.compare( line ) )
                {
                    // If they match then fetch the value associated 
                    // with that key
                    for( int i=0; i<count; ++i ) 
                        cfg >> *(result+i);

                    // And since we don't want anything we can 
                    // reset the file
                    _reset();

                    // and safely return
                    return 1;
                }

                // Do another getline() to clear the line remains
                cfg.getline( line, 50 );

            } // end while( )

            return 0;

        } // end _get()

        /**
         * Returns the file pointer to the beginning of the file
         */
        void _reset()
        {
            cfg.clear();
            cfg.seekg( 0, ios_base::beg );
        }

    }; // end of class Config

} // end of namespace tlib