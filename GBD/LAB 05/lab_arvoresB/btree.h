#ifndef _BTREE_H
#define	_BTREE_H

#include <stdio.h>

/*
 * Definicao da ordem da arvore
 */
#define ORDEM 511

/*
 * Definicao da estrutura de dados do cabecalho. Objetivo do cabecalho é apenas guardar qual numero da pagina raiz da arvore
 */
struct cabecalhoB {
    int paginaRaiz; // numero da pagina raiz da arvore
    int alturaArvore; // altura da arvore
    int numeroElementos; // numero de chaves armazenadas na arvore
    int numeroPaginas; // numero de paginas da arvore
};
typedef struct cabecalhoB cabecalho;
/*
 * Definicao da estrutura de dados das paginas da arvore
 */
struct paginaB {
    int numeroElementos; // numero de elementos na pagina
    int numeroPagina; // vamos guardar o numero da pagina dentro da propria pagina
    int chaves[ORDEM];
    int ponteiros[ORDEM];
};
typedef struct paginaB pagina;

class btree {
public:
    /*
     * Construtor. Abre arquivo de indice.
     */
    btree();

    /*
     * Destrutor. Fecha arquivo de indice.
     */
    virtual ~btree();

    /*
     * Insere par chave e offset para o registro (offset do arquivo de registros) na árvore
     * Tarefas:
     * - localizar pagina para inserir registro
     * - inserir ordenado na pagina
     * - atualizar numeroElementos na pagina
     * - atualizar recursivamente as páginas ancestrais
     */
    void insereChave(int chave, int offsetRegistro);

    /*
     * Remove par chave e offset.
     * Tarefas:
     * - localizar pagina para remover registro
     * - inserir ordenado na pagina
     * - atualizar numeroElementos na pagina
     * - atualizar recursivamente as páginas ancestrais
     */
    void removeChave(int chave);

    /*
     * Busca chave e retorna o offset. Retorna -1 caso nao encontre a chave
     * Tarefas:
     * - localizar pagina
     * - retorna offset
     */
    int buscaChave(int chave);

    /*
     * Retorna numero de elementos armazenado no cabecalho da arvore
     */
    int getNumeroElementos() { return cabecalhoArvore.numeroElementos; }

    /*
     * Retorna altura da arvore armazenada no cabecalho da arvore
     */
    int getAlturaArvore() { return cabecalhoArvore.alturaArvore; }

    /*
     * Retorna o numero medio de elementos por pagina da arvore. Considerar apenas paginas folha
     */
    int computarTaxaOcupacao();

private:
    /*
     * Cabecalho da arvore
     */
    cabecalho cabecalhoArvore;

    /*
     * Instancia para ler uma pagina
     */
    pagina *paginaAtual;

    /*
     * Manipulador do arquivo de dados
     */
    FILE *arquivo;

    /*
     * Criaçao uma nova pagina. Parametro com numero da pagina deve ser passado por referencia (exemplo: int idpagina;
     * btree->novaPagina(&idpagina);) pois no retorno da funçao o idpagina tera o numero da nova pagina.
     */
    pagina *novaPagina(int *idPagina) {
        pagina *pg = new pagina;
        pg->numeroElementos = 0;
        fseek(arquivo, 0, SEEK_END);
        fwrite(pg,sizeof(pg),1,arquivo);
        leCabecalho();
        cabecalhoArvore.numeroPaginas++;
        salvaCabecalho();
        pg->numeroPagina = cabecalhoArvore.numeroPaginas;
        idPagina[0] = cabecalhoArvore.numeroPaginas;
        return pg;
    }

    /*
     * Leitura de uma pagina existente.
     */
    pagina *lePagina(int idPagina) {
        pagina *pg = new pagina;
        fseek(arquivo, (1+idPagina)*sizeof(pg), SEEK_SET);
        fread(pg,sizeof(pg),1,arquivo);
        return pg;
    }

    /*
     * Persistencia de uma pagina.
     */
    void salvaPagina(int idPagina, pagina *pg) {
        fseek(arquivo, (1+idPagina)*sizeof(pg), SEEK_SET);
        fwrite(pg,sizeof(pg),1,arquivo);
    }

    /*
     * Salva o cabecalho
     */
    void salvaCabecalho() {
        fseek(arquivo,0,SEEK_SET);
        fwrite(&cabecalhoArvore,sizeof(cabecalhoArvore),1,arquivo);
    }

    /*
     * Le o cabecalho
     */
    void leCabecalho() {
        fseek(arquivo,0,SEEK_SET);
        fread(&cabecalhoArvore,sizeof(cabecalhoArvore),1,arquivo);
    }

    void buscaRecursao(int chave, int idPagina){
        paginaAtual = lePagina(idPagina);
        int i;

        for(i=0; i < paginaAtual->numeroElementos; i++){
            if(chave == paginaAtual->chaves[i]){
                printf("Achei!");
            }

            if(chave < paginaAtual->chaves[i]){
                if(paginaAtual->ponteiros[i] != -1)
                    buscaRecursao(chave, paginaAtual->ponteiros[i]);
            } else {

                if(chave > paginaAtual->chaves[i]){
                    if(paginaAtual->ponteiros[i+1] != -1)
                        buscaRecursao(chave, paginaAtual->ponteiros[i+1]);
                }
            
            }
        }
    }
};

#endif	/* _BTREE_H */

