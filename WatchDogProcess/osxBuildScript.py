import os
import sys
import argparse
import shutil

os.system('clear')

current_dir = os.path.dirname(os.path.realpath(__file__))
project_dir = os.path.join(current_dir, os.path.pardir)
build_dir = os.path.abspath(os.path.join(current_dir, '../watchdog-build'))
symbol_dir = os.path.join(project_dir, "symbols")

parser = argparse.ArgumentParser(description='Proctortrack App Builder for OS X')
parser.add_argument('-c','--clean', help='Clean build directory', action='store_true')
parser.add_argument('-d','--debug', help='Debug build', action='store_true')
parser.add_argument('-r','--release', help='Release build', action='store_true')
parser.add_argument('-dmg','--dmg', help='Build with dmg file', action='store_true')
parser.add_argument('-v', '--version', metavar='', help='Qt version to use', action='store', default='5.5.1')
parser.add_argument('-g', '--git', metavar= '', help='Git branch to use', action='store')
parser.add_argument('-deploy', '--deploy', help='Deploy (release only)', action='store_true')
parser.add_argument('-z', '--zip', help='Zip binary', action='store_true')
parser.add_argument('-rd', '--remotedesk', help='Build for RemoteDesk', action='store_true')

def build(producttype):
    app_name = '%s.app' % producttype
    dmg_name = '%s.dmg' % producttype
    base_build_dir = 'build-%s-Desktop_Qt_5_5_%s_clang_64bit-' % (producttype, args.version[-1:])

    pro_path = os.path.abspath(os.path.join(current_dir, './WatchDogProcess.pro'))
    qmake_dir = os.path.join(os.path.expanduser("~"), 'Qt/5.5/clang_64/bin/qmake')

    if args.debug:
        build_path = os.path.join(build_dir, base_build_dir + 'Debug/')
        app_path = build_path + app_name

        if not os.path.isdir(build_path): os.makedirs(build_path)
        os.chdir(build_path)
        if args.clean: os.system('make clean')

        qmake_args = ' -r -spec macx-clang CONFIG+=debug CONFIG+=x86_64 CONFIG+=%s' % producttype
        qmake_cmd = qmake_dir + " " + pro_path + qmake_args
        os.system(qmake_cmd)
        os.system('make')

    if args.release:
        build_path = os.path.join(build_dir, base_build_dir + 'Release/')
        app_path = build_path + app_name
        
        if not os.path.isdir(build_path): os.makedirs(build_path)
        os.chdir(build_path)
        if args.clean: os.system('make clean')

        qmake_args = ' -r -spec macx-clang CONFIG+=x86_64 CONFIG+=%s' % producttype
        qmake_cmd = qmake_dir + " " + pro_path + qmake_args
        os.system(qmake_cmd)

        os.system('make')
        os.chdir(current_dir)

        if args.dmg: deploy_args = '-dmg'
        else: deploy_args = ''

        deploy_cmd = "~/Qt/5.5/clang_64/bin/macdeployqt %s %s" % ( app_path, deploy_args)
        symboldump_cmd = "python %s/symbolstore.py %s/mac_dump_syms %s/../symbols %s" % (current_dir, current_dir, current_dir, build_dir)
        sign_cmd = "../tools/sign.sh %s %s" % (app_path, producttype)

        if args.deploy:
            os.system(deploy_cmd)
            #os.system(symboldump_cmd)
            os.system(sign_cmd)
            #os.system(symbol_sync_cmd)

        if args.zip:
            zip_filename = '/%s.zip' % producttype
            zip_cmd = 'ditto -ck --sequesterRsrc --keepParent %s %s' % ((build_path + app_name), (build_dir + zip_filename))
            os.system(zip_cmd)

            update_filename = '/%sUpdate_OSX.zip' % producttype
            shutil.copyfile(build_dir + zip_filename, build_dir + update_filename)


args = parser.parse_args()

if len(sys.argv) == 1:
    args.release = True
    args.deploy = True
    args.zip = True
    args.remotedesk = False

#if args.remotedesk:
#    build("RemoteDesk")
#else:
#    build("Proctortrack")

build("WatchDogProcess")