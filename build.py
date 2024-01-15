import os
import subprocess

os.makedirs('bin/tmp/', exist_ok=True)

c_files = []
for file in os.listdir('src'):
    if file.endswith('.c'):
        c_files.append(os.path.join('src', file))
        print(file)

o_files = []
for file in c_files:
    out = os.path.join('bin', 'tmp', os.path.basename(file.removesuffix(".c") + ".o"))
    command = ['gcc', '-c', '-o', out, file, '-Iinclude', '-I.', '-g']
    print(' '.join(command))
    res = subprocess.run(command)
    try: res.check_returncode()
    except subprocess.CalledProcessError:
        print('Compiler exited with error')
        exit(res.returncode)
    o_files.append(out)

command = ['gcc', '-o', 'bin/gravity', *o_files, '-Lbin', '-l:raylib.dll']
print(' '.join(command))
res = subprocess.run(command)
try: res.check_returncode()
except subprocess.CalledProcessError:
    print('Linker exited with error')
    exit(res.returncode)