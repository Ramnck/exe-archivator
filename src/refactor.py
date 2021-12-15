import sys
"""
I now that this script is terrible, but i am too lazy to do this in a proper way 
"""
def main() :

    try:
        name = sys.argv[1]
    except IndexError:
        name = ".\src\execute-me.c"

    with open(name,'r', encoding = 'utf-8') as f:
        lines = f.readlines()

    lines = [i.replace('                                        ', '') for i in lines]
    lines = [i.replace('\\', '\\\\') for i in lines]
    lines = [i.replace('"', '\\"') for i in lines]
    lines = [i.replace('%s', '%%s') for i in lines]
    lines = [i.replace('\n', '\\n') for i in lines]
    lines = [i.replace('777', '%d') for i in lines]
    
    with open(".\src\source_code.h",'w') as f:
        f.write('#pragma once\nchar source_code[] = "')
        for i in lines:
            if i[0:2] != '//':
                f.write(i)
        f.write('";')
    # print("Refactoring", name, "was done")

if __name__ == "__main__":
    main()