"""
Эта программа конвертирует содержимое файла execute-me.c в char[] строку в переменную source_code, находящуюся в файле source_code.h
I now that this script is terrible, but i am too lazy to to this in a proper way 
"""
with open('execute-me.c','r', encoding = 'utf-8') as f:
    lines = f.readlines()
lines = [i.replace('                                        ', '') for i in lines]
lines = [i.replace('\\', '\\\\') for i in lines]
lines = [i.replace('"', '\\"') for i in lines]
lines = [i.replace('%s', '%%s') for i in lines]
lines = [i.replace('\n', '\\n') for i in lines]
lines = [i.replace('777', '%d') for i in lines]
print(*lines)
with open('source_code.h','w') as f:
    f.write('#pragma once\nchar source_code[] = "')
    for i in lines:
        if i[0:2] != '//':
            f.write(i)
    f.write('";')