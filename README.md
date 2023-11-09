# Sistema de Identificação de Jogadores para SA-MP (Pawn)

Este script implementa um sistema de identificação de jogadores em servidores SA-MP, utilizando uma combinação de IDs atribuídos pelo servidor e IDs fixos provenientes de um banco de dados SQLite. Aqui está uma visão geral do sistema:

## Estrutura de IDs:

- **playerid:** Representa o ID do jogador atribuído pelo servidor SA-MP. Varia de 0 a 999, dependendo do número de slots disponíveis. Valores inválidos são 0xFFFF ou INVALID_PLAYER_ID.

- **staticId:** Refere-se ao ID fixo do jogador armazenado no banco de dados SQLite. Começa a partir de 1 e não tem limites superiores. Valores inválidos são 0 ou INVALID_STATIC_PLAYER_ID.

## Funções Principais:

### 1. GetPlayerStaticID(playerid)

Retorna o ID fixo do jogador com base no ID atribuído pelo servidor. Se o playerid for inválido, retorna INVALID_STATIC_PLAYER_ID.

```pawn
new staticId = GetPlayerStaticID(playerid);
if(staticId != INVALID_STATIC_PLAYER_ID)
{
	printf("'%d' é o ID fixo obtido pelo ID não fixo '%d'", staticId, playerid);
}
else
{
	printf("Não há jogador com esse ID não fixo '%d'", playerid);
}
```

### 2. GetPlayerStaticIDFromName(const name[], bool:offline_too = false)

Retorna o ID fixo do jogador com base no nome. Se o nome for inválido, retorna INVALID_STATIC_PLAYER_ID. Se `offline_too` for verdadeiro, a busca inclui jogadores offline; caso contrário, busca apenas jogadores conectados.

```pawn
new staticId = GetPlayerStaticIDFromName("NomeDoJogador", true);
if(staticId != INVALID_STATIC_PLAYER_ID)
{
	printf("'%d' é o ID fixo obtido pelo nome '%s'", staticId, "NomeDoJogador");
}
else
{
	printf("Não há jogador com esse nome '%s'", "NomeDoJogador");
}
```

### 3. GetPlayerNameFromStaticID(staticId, dest[], len = sizeof dest)

Obtém o nome do jogador através do ID fixo. Retorna 1 se for um ID fixo válido, 0 se inválido.

```pawn
new playerName[MAX_PLAYER_NAME];
if(GetPlayerNameFromStaticID(staticId, playerName))
{
	printf("'%s' é o nome de usuario obtido pelo ID fixo '%d'", playerName, staticId);
}
else
{
	printf("Não há jogador com esse ID fixo '%d'", staticId);
}
```

### 4. GetPlayerIDFromStaticID(staticId)

Obtém o ID do jogador através do ID fixo. Retorna INVALID_PLAYER_ID se o ID fixo for inválido.

```pawn
new playerid = GetPlayerIDFromStaticID(staticId);
if(playerid != INVALID_PLAYER_ID)
{
	printf("'%d' é o ID não fixo obtido pelo ID fixo '%d'", playerid, staticId);
}
else
{
	printf("Não há jogador com esse ID fixo '%d'", staticId);
}
```

Este sistema proporciona uma maneira eficiente de identificar jogadores em um servidor SA-MP, facilitando o gerenciamento de dados e interações entre jogadores.
