import subprocess
import sys
from package_helpers import clone_and_checkout, append_paths, run
import platform

def main():
    build_mode = sys.argv[1]
    install_prefix = sys.argv[2]
    module_destination = sys.argv[3]

    os_name = platform.system().lower()

    package_output_dir = "jwtcpp"
    commit_hash_release = "e71e0c2d584baff06925bbb3aad683f677e4d498"
    jwtcpp_dir = append_paths(module_destination, "modules", package_output_dir)
    build_dir = append_paths(module_destination, "dependencies", os_name, package_output_dir)
    jwtcpp_cmake_dir = jwtcpp_dir

    clone_and_checkout("https://github.com/Thalhammer/jwt-cpp.git", destination=jwtcpp_dir, commit_hash=commit_hash_release)
    
    if os_name == "windows":
        run(f'cmake -S "{jwtcpp_cmake_dir}" -B "{build_dir}" '
            f'-DCMAKE_INSTALL_PREFIX="{install_prefix}" '
            f'-DCMAKE_PREFIX_PATH="{install_prefix}" '
            f'-DJWT_DISABLE_PICOJSON=ON '
            f'-DJWT_BUILD_EXAMPLES=OFF')
        run(f'cmake --build "{build_dir}" --config {build_mode} --target install')
    elif os_name in ["linux", "darwin", "freebsd"]:
        run(f'cmake -S "{jwtcpp_cmake_dir}" -B "{build_dir}" '
            f'-DCMAKE_BUILD_TYPE="{build_mode}" '
            f'-DCMAKE_INSTALL_PREFIX="{install_prefix}" '
            f'-DCMAKE_PREFIX_PATH="{install_prefix}" '
            f'-DJWT_DISABLE_PICOJSON=ON '
            f'-DJWT_BUILD_EXAMPLES=OFF')
        run(f'cmake --build "{build_dir}" --target install')

if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        print(f"Error while running command: {e}", file=sys.stderr)
        sys.exit(1)