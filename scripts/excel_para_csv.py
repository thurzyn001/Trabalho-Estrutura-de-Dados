#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Utilitário Opcional: Conversor de Planilha Excel (.xlsx) para CSV (.csv)
Disciplina: Estrutura de Dados — 4ª Fase de Ciência da Computação

Uso:
    python scripts/excel_para_csv.py minha_planilha.xlsx [saida.csv]

Nota: Funciona usando apenas a biblioteca padrão do Python (zipfile + xml),
não exigindo 'pip install pandas' ou 'openpyxl' no computador do laboratório!
"""

import csv
import os
import sys
import zipfile
import xml.etree.ElementTree as ET


def converter_xlsx_nativo(caminho_xlsx: str, caminho_csv: str) -> None:
    """Converte .xlsx para .csv usando apenas bibliotecas nativas do Python."""
    ns = {"main": "http://schemas.openxmlformats.org/spreadsheetml/2006/main"}

    with zipfile.ZipFile(caminho_xlsx, "r") as z:
        # 1. Carrega a tabela de strings compartilhadas (se existir)
        strings_compartilhadas = []
        if "xl/sharedStrings.xml" in z.namelist():
            arvore_str = ET.fromstring(z.read("xl/sharedStrings.xml"))
            for si in arvore_str.findall("main:si", ns):
                partes = [t.text or "" for t in si.findall(".//main:t", ns)]
                strings_compartilhadas.append("".join(partes))

        # 2. Localiza a primeira aba da planilha (sheet1.xml)
        aba_xml = "xl/worksheets/sheet1.xml"
        if aba_xml not in z.namelist():
            abas = [n for n in z.namelist() if n.startswith("xl/worksheets/sheet")]
            if not abas:
                raise RuntimeError("Nenhuma aba encontrada dentro do arquivo .xlsx.")
            aba_xml = sorted(abas)[0]

        arvore_aba = ET.fromstring(z.read(aba_xml))
        linhas_extraidas = []

        for row in arvore_aba.findall(".//main:sheetData/main:row", ns):
            valores_linha = []
            for cell in row.findall("main:c", ns):
                tipo = cell.attrib.get("t", "")
                v_node = cell.find("main:v", ns)
                valor = v_node.text if (v_node is not None and v_node.text is not None) else ""

                if tipo == "s" and valor.isdigit():
                    idx = int(valor)
                    if 0 <= idx < len(strings_compartilhadas):
                        valor = strings_compartilhadas[idx]
                elif tipo == "inlineStr":
                    t_node = cell.find(".//main:t", ns)
                    valor = t_node.text if (t_node is not None and t_node.text is not None) else ""

                valores_linha.append(valor.strip())

            if any(valores_linha):
                linhas_extraidas.append(valores_linha)

    os.makedirs(os.path.dirname(caminho_csv) or ".", exist_ok=True)
    with open(caminho_csv, "w", newline="", encoding="utf-8") as f_out:
        escritor = csv.writer(f_out, delimiter=";")
        escritor.writerows(linhas_extraidas)

    print(f"[OK] Planilha '{caminho_xlsx}' convertida para '{caminho_csv}' ({len(linhas_extraidas)} linhas).")


def main() -> None:
    if len(sys.argv) < 2:
        print("Uso: python scripts/excel_para_csv.py <planilha.xlsx> [data/dados_excel.csv]")
        sys.exit(1)

    entrada = sys.argv[1]
    saida = sys.argv[2] if len(sys.argv) >= 3 else "data/dados_excel.csv"

    if not os.path.exists(entrada):
        print(f"[ERRO] Arquivo '{entrada}' nao encontrado.")
        sys.exit(1)

    converter_xlsx_nativo(entrada, saida)
    print("\nProximo passo: Consulte o Checklist de 5 Minutos no arquivo TODO.md")
    print("para ajustar as colunas em include/registro.h e src/leitor_csv.c se necessario!")


if __name__ == "__main__":
    main()
