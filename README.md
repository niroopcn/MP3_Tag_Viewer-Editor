# MP3_Tag_Viewer-Editor

MP3 tag Viewer/Editor is a command line tool which will read and display MP3 (ID3) tag information from MP3 files.

Usage:
1. To View mp3 file contents: ./a.out -v mp3_file_name
2. To Edit mp3 file contents: ./a.out -e [OPTIONS] mp3_file_name
   Available edit options are -t/-a/-A/-m/-y/-c
        -t -> Modifies Title tag
        -a -> Modifies Artist tag
        -A -> Modifies Album tag
        -m -> Modifies Genre(Content) tag
        -y -> Modifies Year tag
        -c -> Modifies Comment tag

ID3 is a metadata container most often used in conjunction with the MP3 audio file format. It allows information such as the title, artist, album, track  number, and other information about the file to be stored in the file itself. There are two unrelated versions of ID3: ID3v1 and ID3v2. ID3v2 is the most commonly used version.

ID3v1 has limitations and only 30 characters can be stored. The ID3v2 tag is more flexible and dynamic.

An ID3v2 tag starts with a tag header followed by one or more frames. Each frame in turn consists of a header and (usually) some kind of content.
ID3v2 header tag has a signature code of "ID3x" where x is the sub-version number of the  tag. Typically ID3v2 tags are found at the beginning of an MP3 file.

ID3v2 header container described below (Total 10 bytes)
a. ID3v2/file identifier "ID3" -> (3 bytes)
b. ID3v2 version $03 00 (2, 3 or 4) -> (2 bytes)
c. ID3v2 flags -> (1 bytes)
d. ID3v2 size 4 * %0xxxxxxx (Total size of ID3 tag) -> (4 bytes)

ID3v2.3 or ID3v2.4 Frame header
Frame ID $xx xx xx xx (Four characters)
eg: TALB Album/Movie/Show title, TIT2 Title/songname/content description
Size $xx xx xx xx (Four characters)
Flags $xx xx (2 bytes)

Compatability: This tool works with ID3v2.3 and ID3v2.4
