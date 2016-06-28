
import os

import dirs

EXTS = ['.cpp',
        '.h',
        '.cc',
        '.c',
        '.l',
        '.y',
        '.py',
        '.txt',
        '.cmake',
        '.org',
        '.bat',
        '.sh',
        '.html',
        '.doxy',
        '.def',
        '.atl',
]

for subdir, direc, files in os.walk(dirs.PROJECT_DIR):
    for file in files:
        p = os.path.join(subdir, file).lower()
        if os.path.splitext(p)[1] in EXTS:
            with open(p, 'rb') as f:
                d = f.read()
                drep = d.replace("\r", "")
            if drep != d:
                print "Fixing " + p
                with open(p, 'wb') as f:
                    f.write(drep)

                
