def loadFile(file: str):
    with open(file, 'rb') as f:
        return f.read()


if __name__ == '__main__':
    simple_vert = f'{loadFile("./simple.vert")}'
    simple_frag = f'{loadFile("./simple.frag")}'
    model_vert = f'{loadFile("./model.vert")}'
    model_frag = f'{loadFile("./model.frag")}'
    model_tcs = f'{loadFile("./model.tesc")}'
    model_tes = f'{loadFile("./model.tese")}'
    model_gs = f'{loadFile("./model.geom")}'

    with open('./shaderBuf.h', 'w') as f:
        f.write(f'#pragma once\n'
                f'\n'
                f'#define simple_vert "{simple_vert[2:-1]}"\n'
                f'\n'
                f'#define simple_frag "{simple_frag[2:-1]}"\n'
                f'\n'
                f'#define model_vert "{model_vert[2:-1]}"\n'
                f'\n'
                f'#define model_frag "{model_frag[2:-1]}"\n'
                f'\n'
                f'#define model_tcs "{model_tcs[2:-1]}"\n'
                f'\n'
                f'#define model_tes "{model_tes[2:-1]}"\n'
                f'\n'
                f'#define model_gs "{model_gs[2:-1]}"\n'
                f'')
