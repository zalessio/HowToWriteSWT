
/*
    Copyright 2012 Andrew Perrault and Saurav Kumar.

    This file is part of DetectText.

    DetectText is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DetectText is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DetectText.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cassert>
#include "../include/imageio.h"
#include "../include/TextDetection.h" 



int mainTextDetection ( int argc, char * * argv )
{
  IplImage * byteQueryImage = loadByteImage ( argv[1] );
  if ( !byteQueryImage )
  {
    printf ( "couldn't load query image\n" );
    return -1;
  }

  // Detect text in the image
  textDetection ( byteQueryImage, atoi(argv[2]) );
  
  cvReleaseImage ( &byteQueryImage );
  return 0;
}


int main ( int argc, char * * argv )
{
  if ( ( argc != 3 ) )
  {
    printf ( "ERROR using: %s !! Not correct input argument:  #imagefile  #darkText \n",
             argv[0] );

    return -1;
  }
  return mainTextDetection ( argc, argv );
}
