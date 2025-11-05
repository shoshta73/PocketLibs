project('expr_eval', 'cpp',
        version: '1.0.0',
        default_options: [
                'cpp_std=c++23',
                'warning_level=3',
                'cpp_rtti=false'  # Compile without RTTI
        ]
)

# Download casting.hxx if it doesn't exist
casting_header = meson.current_source_dir() / 'include' / 'casting.hxx'
if not run_command('test', '-f', casting_header, check: false).returncode() == 0
        message('Downloading casting.hxx...')
        run_command('mkdir', '-p', meson.current_source_dir() / 'include', check: true)
        run_command('curl', '-L', '-o', casting_header,
                'https://raw.githubusercontent.com/shoshta73/PocketLibs/main/casting.hxx',
                check: true)
endif

# Include directory
inc = include_directories('include')

# Compile definitions
cpp_args = ['-DCASTING_NAMESPACE=ExprEval']

# Platform-specific compiler flags
if meson.get_compiler('cpp').get_id() == 'msvc'
        cpp_args += ['/GR-']  # Disable RTTI for MSVC
else
        cpp_args += ['-fno-rtti']  # Disable RTTI for GCC/Clang
endif

# Create executable
executable('expr_eval',
        'src/main.cxx',
        include_directories: inc,
        cpp_args: cpp_args
)
