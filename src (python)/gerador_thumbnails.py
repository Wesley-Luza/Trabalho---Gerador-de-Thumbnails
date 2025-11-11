import os
import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image

def redimensiona_imagem(input_path, output_path, novo_width, novo_height):
    """Redimensiona a imagem usando interpolação bilinear"""
    try:
        with Image.open(input_path) as img:
            img_redimensionada = img.resize((novo_width, novo_height), Image.BILINEAR)
            img_redimensionada.save(output_path)
            return f"✅ {os.path.basename(output_path)} salva."
    except Exception as e:
        return f"❌ Erro: {os.path.basename(input_path)} ({e})"

def tem_extensao_valida(filename):
    """Verifica se o arquivo é jpg, jpeg ou png"""
    ext = os.path.splitext(filename)[1].lower()
    return ext in [".jpg", ".jpeg", ".png"]

def eh_thumbnail(filename):
    """Ignora arquivos que já começam com 'thumb_'"""
    return filename.lower().startswith("thumb_")

def processa_pasta(diretorio, novo_width, novo_height, text_log):
    """Percorre a pasta e gera thumbnails"""
    if not os.path.isdir(diretorio):
        messagebox.showerror("Erro", f"Diretório inválido: {diretorio}")
        return

    resultados = []

    for arquivo in os.listdir(diretorio):
        input_path = os.path.join(diretorio, arquivo)

        # Ignora pastas, arquivos sem extensão válida e já processados
        if not os.path.isfile(input_path):
            continue
        if eh_thumbnail(arquivo) or not tem_extensao_valida(arquivo):
            continue

        output_path = os.path.join(diretorio, f"thumb_{arquivo}")
        resultado = redimensiona_imagem(input_path, output_path, novo_width, novo_height)
        resultados.append(resultado)

    # Exibe no log
    text_log.delete(1.0, tk.END)
    if resultados:
        text_log.insert(tk.END, "\n".join(resultados))
        messagebox.showinfo("Concluído", "Thumbnails geradas com sucesso!")
    else:
        text_log.insert(tk.END, "Nenhuma imagem válida encontrada.")
        messagebox.showinfo("Aviso", "Nenhuma imagem válida encontrada no diretório.")

def escolher_pasta(entry_dir):
    """Abre o seletor de pasta"""
    pasta = filedialog.askdirectory(title="Selecione a pasta de imagens")
    if pasta:
        entry_dir.delete(0, tk.END)
        entry_dir.insert(0, pasta)

def gerar(entry_dir, entry_tam, text_log):
    """Lê dados da interface e inicia o processo"""
    diretorio = entry_dir.get().strip()
    tamanho_str = entry_tam.get().strip()

    if not diretorio:
        messagebox.showwarning("Aviso", "Selecione uma pasta de imagens.")
        return
    if not tamanho_str:
        messagebox.showwarning("Aviso", "Informe o tamanho (ex: 128 ou 50x100).")
        return

    try:
        if "x" in tamanho_str.lower():
            largura, altura = map(int, tamanho_str.lower().split("x"))
        else:
            largura = altura = int(tamanho_str)
    except ValueError:
        messagebox.showerror("Erro", "Formato inválido! Use 128 ou 50x100.")
        return

    processa_pasta(diretorio, largura, altura, text_log)

# ====== Interface Gráfica ======
root = tk.Tk()
root.title("Gerador de Thumbnails")
root.geometry("520x420")
root.resizable(False, False)
root.configure(bg="#f2f2f2")

# Título
tk.Label(root, text="🖼️ Gerador de Thumbnails", font=("Segoe UI", 16, "bold"), bg="#f2f2f2").pack(pady=10)

# Escolher diretório
frame_dir = tk.Frame(root, bg="#f2f2f2")
frame_dir.pack(pady=10)
tk.Label(frame_dir, text="Pasta de imagens:", bg="#f2f2f2").pack(side=tk.LEFT, padx=5)
entry_dir = tk.Entry(frame_dir, width=40)
entry_dir.pack(side=tk.LEFT, padx=5)
tk.Button(frame_dir, text="📂 Selecionar", command=lambda: escolher_pasta(entry_dir)).pack(side=tk.LEFT)

# Campo tamanho
frame_tam = tk.Frame(root, bg="#f2f2f2")
frame_tam.pack(pady=10)
tk.Label(frame_tam, text="Tamanho (ex: 128 ou 50x100):", bg="#f2f2f2").pack(side=tk.LEFT, padx=5)
entry_tam = tk.Entry(frame_tam, width=10)
entry_tam.pack(side=tk.LEFT)

# Botão gerar
tk.Button(root, text="🚀 Gerar Thumbnails", bg="#0078D7", fg="white",
          font=("Segoe UI", 10, "bold"),
          command=lambda: gerar(entry_dir, entry_tam, text_log)
).pack(pady=15)

# Caixa de log
text_log = tk.Text(root, width=60, height=12, wrap="word", bg="#ffffff", fg="#333")
text_log.pack(padx=10, pady=10)

# Rodapé
tk.Label(root, text="Desenvolvido em Python + Pillow", font=("Segoe UI", 8), bg="#f2f2f2", fg="#555").pack(pady=5)

root.mainloop()