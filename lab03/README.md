# Relatório Laboratório 03
Universidade Federal do Rio de Janeiro | Ciência da Computação | Computação Concorrente 2021.1


## Configurações do hardware

- **MacBook Pro** (13-inch, Mid 2012)
- **macOS Catalina** Versão 10.15.7
- **Processador** 2,5 GHz Intel Core i5 Dual-Core (3 MB de cache L3)
- **Memória** 4 GB DDR3, 1600 MHz
- **Gráficos** Intel HD Graphics 4000 1536 MB


## Tempos de execução

| Tamanho do Vetor | Nº de Threads | Tempo Sequencial (s) | Tempo Concorrente (s) | Razão de Aceleração |
| ------------- | ------------- | ------------- | ------------- | ------------- |
| 10<sup>5</sup>  | 2 | 0.001917 | 0.000631 | 3.038035 |
| 10<sup>5</sup>  | 4 | 0.002105 | 0.000896 | 2.349330 |
| 10<sup>7</sup>  | 2 | 0.201522 | 0.055435 | 3.635285 |
| 10<sup>7</sup>  | 4 | 0.189849 | 0.081859 | 2.319220 |
| 10<sup>9</sup>  | 2 | *** | *** | *** |
| 10<sup>9</sup>  | 4 | *** | *** | *** |

*** Não consegui os dados desses testes, o computador reinicia e me impossibilita de fazê-los.
