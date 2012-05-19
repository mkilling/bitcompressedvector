from fabric.api import *

env.hosts = ['mkilling@192.168.31.39']
env.output_prefix = False

@task(default=True)
def profile_on_server():
    run('mkdir -p bitcompressedvector')
    with cd('bitcompressedvector'):
        run('rm -rf *')
        put('*', '.')
        run('make test')
