#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

// Função para interpolação bilinear simples
unsigned char *redimensiona_bilinear(
    unsigned char *input,
    int width, int height, int channels,
    int novo_width, int novo_height
) {
    unsigned char *output = malloc(novo_width * novo_height * channels);
    if (!output) return NULL;

    float x_ratio = (float)(width - 1) / novo_width;
    float y_ratio = (float)(height - 1) / novo_height;

    for (int j = 0; j < novo_height; j++) {
        for (int i = 0; i < novo_width; i++) {
            float x = x_ratio * i;
            float y = y_ratio * j;
            int x_l = (int)x;
            int y_l = (int)y;
            int x_h = (x_l + 1 < width) ? x_l + 1 : x_l;
            int y_h = (y_l + 1 < height) ? y_l + 1 : y_l;

            float x_weight = x - x_l;
            float y_weight = y - y_l;

            for (int c = 0; c < channels; c++) {
                float a = input[(y_l * width + x_l) * channels + c];
                float b = input[(y_l * width + x_h) * channels + c];
                float d = input[(y_h * width + x_l) * channels + c];
                float e = input[(y_h * width + x_h) * channels + c];

                float pixel =
                    a * (1 - x_weight) * (1 - y_weight) +
                    b * x_weight * (1 - y_weight) +
                    d * (1 - x_weight) * y_weight +
                    e * x_weight * y_weight;

                output[(j * novo_width + i) * channels + c] = (unsigned char)pixel;
            }
        }
    }

    return output;
}

// Redimensiona e salva a imagem
void redimensiona_imagem(const char *input_path, const char *output_path, int novo_width, int novo_height) {
    int width, height, channels;
    unsigned char *img = stbi_load(input_path, &width, &height, &channels, 0);
    if (!img) {
        printf("❌ Erro ao carregar imagem: %s\n", input_path);
        return;
    }

    unsigned char *thumb = redimensiona_bilinear(img, width, height, channels, novo_width, novo_height);
    if (!thumb) {
        printf("❌ Erro ao alocar memória para thumbnail.\n");
        stbi_image_free(img);
        return;
    }

    if (stbi_write_png(output_path, novo_width, novo_height, channels, thumb, novo_width * channels))
        printf("✅ Thumbnail salvo: %s\n", output_path);
    else
        printf("❌ Falha ao salvar thumbnail: %s\n", output_path);

    stbi_image_free(img);
    free(thumb);
}

// Verifica se o arquivo tem extensão válida
int tem_extensao_valida(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return 0;
    ext++;
    char lower[10];
    int i;
    for (i = 0; i < 9 && ext[i]; i++)
        lower[i] = tolower(ext[i]);
    lower[i] = '\0';
    return strcmp(lower, "jpg") == 0 || strcmp(lower, "jpeg") == 0 || strcmp(lower, "png") == 0;
}

// Verifica se o arquivo já é uma thumbnail (prefixo "thumb_")
int eh_thumbnail(const char *filename) {
    return strncmp(filename, "thumb_", 6) == 0;
}

// Processa todos os arquivos do diretório
void processa_pasta(const char *diretorio, int novo_width, int novo_height) {
    DIR *dp = opendir(diretorio);
    if (!dp) {
        printf("❌ Erro ao abrir diretório: %s\n", diretorio);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dp))) {
        // Ignora "." e ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Ignora thumbs já criadas
        if (eh_thumbnail(entry->d_name))
            continue;

        char input_path[1024];
        snprintf(input_path, sizeof(input_path), "%s/%s", diretorio, entry->d_name);

        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) continue;
        if (!S_ISREG(path_stat.st_mode)) continue;  // Ignora subpastas

        if (tem_extensao_valida(entry->d_name)) {
            char output_path[1024];
            snprintf(output_path, sizeof(output_path), "%s/thumb_%s", diretorio, entry->d_name);
            redimensiona_imagem(input_path, output_path, novo_width, novo_height);
        }
    }

    closedir(dp);
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <diretório> <tamanho_thumbnail>\n", argv[0]);
        printf("Exemplo: %s imagens 128 ou %s imagens 50x100\n", argv[0], argv[0]);
        return 1;
    }

    const char *diretorio = argv[1];
    int novo_width = 0, novo_height = 0;

    // Tenta ler no formato LxA (ex: 50x100)
    if (sscanf(argv[2], "%dx%d", &novo_width, &novo_height) != 2) {
        // Se o formato for só um número (ex: 128), usa quadrado
        novo_width = novo_height = atoi(argv[2]);
    }

    if (novo_width <= 0 || novo_height <= 0) {
        printf("❌ Tamanho inválido. Use, por exemplo: 128 ou 50x100\n");
        return 1;
    }

    printf("📸 Gerando thumbnails de %dx%d em '%s'...\n", novo_width, novo_height, diretorio);
    processa_pasta(diretorio, novo_width, novo_height);
    return 0;
}