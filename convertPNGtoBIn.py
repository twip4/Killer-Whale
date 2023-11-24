from PIL import Image
import numpy as np

img = Image.open('img_montre/minuteur.png')
im = np.asarray(img)

R5 = (im[..., 0] >> 3).astype(np.uint16) << 11
G6 = (im[..., 1] >> 2).astype(np.uint16) << 5
B5 = (im[..., 2] >> 3).astype(np.uint16)

RGB565 = R5 | G6 | B5

# Créer une fonction vectorisée pour convertir en hexadécimal
def to_hex(val):
    return '0X{:04X}'.format(val)

# Appliquer la conversion à chaque élément du tableau
hex_values = np.vectorize(to_hex)(RGB565)

def write_hex_to_c_file(hex_list, output_file):
    try:
        with open(output_file, 'w') as f:
            f.write('#include <stdint.h>\n')
            f.write('const uint16_t image_data[] = {\n')

            for hex_value in hex_list:
                for x in hex_value :
                    f.write(f"{str(x)},")
                f.write("\n");

            f.write('};\n')

        print(f"Données écrites dans le fichier {output_file}")

    except Exception as e:
        print(f"Erreur : {str(e)}")
# Exemple d'impression des valeurs hexadécimales

write_hex_to_c_file(hex_values,"image_data.c")