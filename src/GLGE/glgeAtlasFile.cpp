/**
 * @file glgeAtlasFile.cpp
 * @author JuNi4
 * @brief Definitions for glgeAtlasFile.hpp
 * @version 0.1
 * @date 2023-12-18
 * 
 * @copyright Copyright (c) JuNi4 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "glgeAtlasFile.hpp"
#include "GLGEInternal/glgeErrors.hpp"

void printd(std::string data){for(ulong i=0;i<data.size();i++){if(data[i]!=0){std::cout<<data[i];}else{std::cout<<" ";}}std::cout<<"<-end\n";}
void printdi(std::string data){for(ulong i=0;i<data.size();i++){std::cout<<(int)data[i];}std::cout<<"<-end\n";}

GLGEAtlasImage::GLGEAtlasImage( std::string name, vec2 pos, vec2 size )
{
    this->name = name;
    this->pos = pos;
    this->size = size;
}

// Main constructor
GLGEAtlasFile::GLGEAtlasFile() {}

// open a file
bool GLGEAtlasFile::open( std::string fp, const char mode )
{
    // check if the file exists
    if ( mode == 'r' && !std::filesystem::is_regular_file( fp ) )
    {
        // if not, return false
        printf( GLGE_ERROR_FILE_NOT_FOUND, fp.c_str() );
        return false;
    }

    // check if the file operation mode is valid
    if ( mode != 'r' && mode != 'w' )
    {
        // if not, exit
        printf( GLGE_ERROR_ATLAS_FILE_INVALID_MODE, &mode );
        exit(1);
    }

    // open the file
    if ( mode == 'r' )
    {
        this->infile = std::ifstream(fp);
    }
    else if ( mode == 'w' )
    {
        this->outfile = std::ofstream(fp);
    }
    
    // save operation mode
    this->mode = mode;

    this->fp = fp;

    // appearently, nothing went wrong...?
    return true;
}

// write to a file
bool GLGEAtlasFile::write()
{
    // check if the file is open and in the correct mode
    if ( mode != 'w' )
    {
        printf(GLGE_ERROR_WRONG_MODE_FOR_ACTION);
        return false;
    }

    // write header //

    this->outfile << GLGE_ATLAS_FILE_HEADER;

    // write number of images
    this->writeInt( this->images.size() );

    // write the width and height
    this->writeInt( (int) this->size.x );
    this->writeInt( (int) this->size.y );

    // Write the images //
    for ( ulong i = 0; i < this->images.size(); i++ )
    {
        // name length
        this->writeInt( this->images[i].name.size() );

        // name itself
        this->outfile << this->images[i].name;

        // x and y
        this->writeInt( (int) this->images[i].pos.x );
        this->writeInt( (int) this->images[i].pos.y );
        // width and height
        this->writeInt( (int) this->images[i].size.x );
        this->writeInt( (int) this->images[i].size.y );
    }

    // everything has gone right apearantly
    return true;
}

// read from a file
bool GLGEAtlasFile::read()
{
    // read the full contents of the file
    std::string data = "";
    std::string line = "";

    while ( std::getline( this->infile, line ) ) { data += line; }

    // Read the Header //

    // header
    if ( data.substr(0,16) != GLGE_ATLAS_FILE_HEADER ) {
        printf(GLGE_ERROR_ATLAS_INVALID, this->fp.c_str());
        return false;
    }

    // atlas info //

    // texture count
    int textureCount = this->unpackInt( data.substr(16,4) );

    // width and height
    this->size.x = this->unpackInt( data.substr(20,4) );
    this->size.y = this->unpackInt( data.substr(24,4) );

    // read textures //
    int start = 28;
    for ( int i = 0; i < textureCount; i++ )
    {
        // image data
        GLGEAtlasImage img;
        // read the first four bits for the name lenght
        int nl = this->unpackInt( data.substr(start,4) ); start += 4;
        // read the name
        img.name = data.substr( start, nl ); start += nl;
        // read the pos
        img.pos.x = this->unpackInt( data.substr(start, 4) ); start+=4;
        img.pos.y = this->unpackInt( data.substr(start, 4) ); start+=4;

        // read width and height
        img.size.x = this->unpackInt( data.substr(start, 4) ); start+=4;
        img.size.y = this->unpackInt( data.substr(start, 4) ); start+=4;

        this->images.push_back(img);
    }

    // everything has gone right apearantly... somehow
    return true;
}

// close the file
bool GLGEAtlasFile::close()
{
    // close file
    this->outfile.close();
    this->infile.close();

    this->mode = '-';
    this->fp = "";

    return true;
}

// add image
void GLGEAtlasFile::addImage( std::string name, vec2 pos, vec2 size )
{
    GLGEAtlasImage x;
    x.name = name;
    x.pos = pos;
    x.size = size;

    this->images.push_back(x);
}

void GLGEAtlasFile::addImage( GLGEAtlasImage image )
{
    this->images.push_back( image );
}

// find index by name
ulong GLGEAtlasFile::find( std::string name )
{
    // go through all images and if the name is the same as the input, return its index
    for ( ulong i = 0; i < this->images.size(); i++ )
    {
        if ( this->images[i].name == name ) { return i; }
    }

    // if nothing was found
    return -1;
}

std::vector<uint8_t> GLGEAtlasFile::packInt( int number, int numBytes )
{
    // output variable
    std::vector<uint8_t> out = {};

    // go through all the bytes and add them to the output
    for ( int i = numBytes-1; i >= 0; i-- )
    {
        // shift the number by the index * 8 and add the result to out
        out.push_back( (number >> i*8) & 0xff );
    }

    // return collected data
    return out;

}

void GLGEAtlasFile::writeInt( int num, int numBytes )
{
    // get the bytes from the int and write them to the file
    std::vector<uint8_t> b = this->packInt( num, numBytes );
    for ( ulong i = 0; i < b.size(); i++ ) { this->outfile << b[i]; }
}

int GLGEAtlasFile::unpackInt( std::string inp )
{
    // go through all the charactars and add them to the final integer
    int out = 0;
    for ( ulong i = 0; i < inp.size(); i++ ) { out += inp[i] << (inp.size()-i-1)*8; }
    return out;
}