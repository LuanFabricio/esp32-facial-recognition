# Notas
- Para usar no modo CLI (imagens estáticas no lugar da câmera)
```c
// main/esp_main.h
...
#define CLI_ONLY_INFERENCE 1
...
```
- Para alterar o modelo utilizado, basta trocar os bytes do array `g_person_detect_model_data` pelos bytes do seu modelo (`.tflite`).
    - Para gerar os bytes do modelo `.tflite`, basta usar o comando `xxd -i <modelo>.tflite > <modelo>.cc`.
    - Se lembre que isso altera os valores esperados pelo resto do programa.

# Problemas encontrados
- Tamanho da partição, quando um modelo maior é usado.
    - Solução: Aumentar o tamanho do `factory` no arquivo `partitions.csv`.
- `heap_allocator_malloc` usando memória interna do ESP32, no lugar da SPRAM.
    - Solução: Utilizar flag `MALLOC_CAP_SPIRAM` como flag.
- Problema para fazer parse do opcode `MUL`
    - Solução: Adicionar os opcodes manualmente na variável `micro_op_resolver` (`micro_op_resolver.Add<Opcode>`), no arquivo `src/main_functions.cc`.
        - Ver arquivo `.../lite/micro/micro_op_resolver.cc`
        ```
        Didn't find op for builtin opcode 'MUL'
        Failed to get registration from op code MUL
        AllocateTensors() failed
        ```
        ```
        Didn't find op for builtin opcode 'PRELU'
        Failed to get registration from op code PRELU

        AllocateTensors() failed
        ```
- Adicionar uma nova imagem na "base":
    - Coloque a imagem na pasta `static_images/sample_images/<nome_da_imagem>`
    - Adicione a imagem no arquivo de build (`static_images/CMakeLists.txt`)
        - A adicione na seção `EMBED_FILES`
    - **Atenção!**
        - Caso a dimensão da imagem seja alterada, você deve alterar o arquivo `main/model_settings.h`, para satisfazer as dimensões da imagem.

# Referências
- Projeto base: https://github.com/espressif/esp-tflite-micro/tree/master/examples/person_detection
