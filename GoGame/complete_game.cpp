#include "complete_game.h"

#include "conio2.h"
#include "constants.h"
#include "go_game.h"
#include "gui_utils.h"
#include "user_input.h"
#include <cstdio>
#include <cstring>

void CompleteGame::setGameStatusMessage(MoveResult result) {
    char message[GAME_STATUS_MAX_LENGTH + 1] = "";
    switch (result) {
    case done:
        strcpy(message, "");
        break;
    case occupied:
        strcpy(message, OCCUPIED_MESSAGE);
        break;
    case suicidal:
        strcpy(message, SUICIDE_MESSAGE);
        break;
    case already_placed:
        strcpy(message, ALREADY_PLACED_MESSAGE);
        break;
    case ko:
        strcpy(message, KO_MESSAGE);
        break;
    case handicap:
        strcpy(message, HANDICAP_MESSAGE);
        break;
    }
    strcpy(gameStatusMessage, message);
}

void CompleteGame::clearGameStatusMessage() {
    strcpy(gameStatusMessage, "");
}

int CompleteGame::getBoardviewSize() const {
    const int buffer_limit = min(mainBuffer.getWidth() - LEGEND_WIDTH - 2, mainBuffer.getHeight() - 2);
    return min(game.getBoardSize() + 2, buffer_limit);
}

void CompleteGame::drawBoard() {
    if (boardBuffer.getWidth() != getBoardviewSize()) {
        boardBuffer = Canvas(getBoardviewSize(), getBoardviewSize());
    }
    boardBuffer.clear();

    const int offset_x = cursor.getViewboxPositionX();
    const int offset_y = cursor.getViewboxPositionY();

    for (int x = 0; x < getBoardviewSize(); ++x) {
        for (int y = 0; y < getBoardviewSize(); ++y) {
            BoardSpace space = game.getSpace(offset_x + x, offset_y + y);
            boardBuffer.setPixel(x, y, getBoardSign(space));
            if (game.getChosenX() >= 0 && offset_x + x == game.getChosenX() && offset_y + y == game.getChosenY()) {
                boardBuffer.setPixelBackground(x, y, YELLOW);
                boardBuffer.setPixelForeground(x, y, BLACK);
            }
        }
    }

    boardBuffer.setPixelBackground(cursor.getLocalX(), cursor.getLocalY(), RED);
}

Pixel CompleteGame::getBoardSign(const BoardSpace& space) {
    switch (space.state) {
    case blackStone:
        return { BLACK_STONE_SIGN, BLACK_STONE_COLOR, BLACK_STONE_BACKGROUND };
    case whiteStone:
        return { WHITE_STONE_SIGN, WHITE_STONE_COLOR, WHITE_STONE_BACKGROUND };
    case edge:
        return { BORDER_SIGN, BORDER_COLOR, BORDER_BACKGROUND };
    case empty:
    default:
        return { EMPTY_SIGN, EMPTY_COLOR, EMPTY_BACKGROUND };
    }
}

void CompleteGame::drawLegend() {
    legendBuffer.clear();
    // show constant data
    legendBuffer.drawText(INSTRUCTIONS, INSTRUCTIONS_POSITION_X, INSTRUCTIONS_POSITION_Y);
    legendBuffer.drawText(PERSONAL_DATA, PERSONAL_DATA_POSITION_X, PERSONAL_DATA_POSITION_Y);
    legendBuffer.drawText(DONE_LIST, DONE_LIST_POSITION_X, DONE_LIST_POSITION_Y);

    // show current score
    char score_message[SCORE_MESSAGE_MAX_LENGTH + 1];
    sprintf(score_message, SCORE_MESSAGE_FORMAT, (int)game.getScoreBlack(), (int)game.getScoreWhite(), (float)game.getScoreWhiteBonus());
    legendBuffer.drawText(score_message, SCORE_MESSAGE_POSITION_X, SCORE_MESSAGE_POSITION_Y);

    // show player's cursor location
    char location_message[LOCATION_MESSAGE_MAX_LENGTH + 1];
    sprintf(location_message, LOCATION_MESSAGE_FORMAT, (int)(cursor.getX() + 1), (int)(cursor.getY() + 1));
    legendBuffer.drawText(location_message, LOCATION_MESSAGE_POSITION_X, LOCATION_MESSAGE_POSITION_Y);

    // show game status
    legendBuffer.drawText(gameStatusMessage, GAME_STATUS_POSITION_X, GAME_STATUS_POSITION_Y, GAME_STATUS_COLOR);
}

void CompleteGame::drawAll() {
    if (mainBuffer.getWidth() != GUIUtils::getTerminalWidth()
        || mainBuffer.getHeight() != GUIUtils::getTerminalHeight()) {
        mainBuffer = Canvas(GUIUtils::getTerminalWidth(), GUIUtils::getTerminalHeight());
    }

    drawBoard();
    drawLegend();

    mainBuffer.clear();

    if (BOARD_OFFSET >= 0) {
        // legend on the right
        mainBuffer.drawCanvas(legendBuffer, ALL_OFFSET, LEGEND_POSITION_TOP);
        const int board_x = legendBuffer.getWidth() + BOARD_OFFSET;
        mainBuffer.drawCanvas(boardBuffer, board_x, BOARD_POSITION_TOP);
    }
    else {
        // legend on the left
        mainBuffer.drawCanvas(boardBuffer, ALL_OFFSET, BOARD_POSITION_TOP);
        const int legend_x = boardBuffer.getWidth() - BOARD_OFFSET - 1;
        mainBuffer.drawCanvas(legendBuffer, legend_x, LEGEND_POSITION_TOP);
    }
}

void CompleteGame::printGame() {
    drawAll();
    mainBuffer.print();
}

void CompleteGame::showAlert(const char* alert_message) {
    mainBuffer.clear();
    mainBuffer.drawText(alert_message, 0, 0);
    mainBuffer.print();
    UserInput::getKey();
}

void CompleteGame::getFilenameFromUser(char* dest, const char* header_message) {
    mainBuffer.clear();
    mainBuffer.drawText(header_message, 0, 0);
    const int xEndPos = mainBuffer.drawText(FILENAME_QUERY, 0, 1);
    mainBuffer.print();
    gotoxy(xEndPos + 1, 2);
    return UserInput::getFilename(dest, FILENAME_MAX_LENGTH);
}

int CompleteGame::getBoardSizeFromUser() {
    mainBuffer.clear();
    mainBuffer.drawText(CHOOSE_BOARD_SIZE_HEADER, 0, 0);
    const int xEndPos = mainBuffer.drawText(BOARD_SIZE_QUERY, 0, 1);
    mainBuffer.print();
    gotoxy(xEndPos + 1, 2);
    return UserInput::getNumber(MIN_GAMEBOARD_SIZE, MAX_GAMEBOARD_SIZE);
}

int CompleteGame::askBoardSize() {
    mainBuffer.clear();
    mainBuffer.drawText(CHOOSE_BOARD_SIZE_HEADER, 0, 0);

    const int choiceLimit = sizeof(BOARD_SIZES) / sizeof(BOARD_SIZES[0]);
    int chosen = 0;

    bool dialog_loop = true;
    while (dialog_loop) {
        // print choices
        int offset = 0;
        unsigned char background_color, foreground_color;
        for (int i = 0; i < choiceLimit; ++i) {
            if (i == chosen) {
                background_color = WHITE;
                foreground_color = BLACK;
            }
            else {
                background_color = BLACK;
                foreground_color = WHITE;
            }
            const int xEndPos = mainBuffer.drawText(BOARD_SIZES[i], offset, 1, foreground_color, background_color);
            offset = xEndPos + 1;
        }

        mainBuffer.print();

        // basic menu for moving between choices
        switch (UserInput::getKey()) {
        case KEY_LEFT:
            if (chosen > 0) {
                chosen--;
            }
            break;
        case KEY_RIGHT:
            if (chosen < choiceLimit - 1) {
                chosen++;
            }
            break;
        case KEY_CONFIRM:
            dialog_loop = false;
            break;
        }
    }

    // other board size
    if (BOARD_SIZE_VALUES[chosen] < 0) {
        return getBoardSizeFromUser();
    }

    return BOARD_SIZE_VALUES[chosen];
}

void CompleteGame::showGameResult() {
    mainBuffer.clear();
    char score_message[SCORE_MESSAGE_MAX_LENGTH + 1];
    sprintf(score_message, SCORE_MESSAGE_FORMAT, (int)game.getScoreBlack(), (int)game.getScoreWhite(), (float)game.getScoreWhiteBonus());
    mainBuffer.drawText(score_message, 0, 1);
    char result_message[WINNING_MESSAGE_MAX_LENGTH + 1] = "";
    switch (game.whoWon()) {
    case black:
        strcpy(result_message, BLACK_WON_MESSAGE);
        break;
    case white:
        strcpy(result_message, WHITE_WON_MESSAGE);
        break;
    default:
        break;
    }
    mainBuffer.drawText(result_message, 0, 2);
    mainBuffer.drawText(GAME_FINISH_DIALOG_INSTRUCTIONS, 0, 3);
    mainBuffer.print();

    bool dialog_loop = true;
    while (dialog_loop) {
        dialog_loop = false;
        switch (UserInput::getKey()) {
        case KEY_NEW_GAME:
            createNewGame();
            break;
        case KEY_CANCEL:
            game.cancelPlacement();
            break;
        case KEY_QUIT:
            gameIsRunning = false;
            break;
        default:
            dialog_loop = true;
            break;
        }
    }
}

void CompleteGame::createNewGame() {
    const int board_size = askBoardSize();
    game = GoGame(board_size);
    cursor = Cursor(game.getBoardSize(), game.getBoardSize(), getBoardviewSize(), getBoardviewSize());
    clearGameStatusMessage();
}

void CompleteGame::saveGame() {
    char filename[FILENAME_MAX_LENGTH + 1];
    getFilenameFromUser(filename, SAVING_HEADER);
    FILE* savefile = fopen(filename, "wb");
    if (savefile == NULL) {
        showAlert(SAVE_ERROR);
        return;
    }
    game.save(savefile);
    fclose(savefile);
    showAlert(SAVE_SUCCESS);
}

void CompleteGame::loadGame() {
    char filename[FILENAME_MAX_LENGTH + 1];
    getFilenameFromUser(filename, LOADING_HEADER);
    FILE* loadfile = fopen(filename, "rb");
    if (loadfile == NULL) {
        showAlert(LOAD_ERROR);
        return;
    }
    game.load(loadfile);
    cursor = Cursor(game.getBoardSize(), game.getBoardSize(), getBoardviewSize(), getBoardviewSize());
    fclose(loadfile);
    showAlert(LOAD_SUCCESS);
}

void CompleteGame::gameInit() {
    GUIUtils::initializeGui(PERSONAL_DATA);
    createNewGame();
    printGame();
}

void CompleteGame::gameLoop() {
    int input_key = UserInput::getKey();
    switch (input_key) {
    case KEY_QUIT:
        gameIsRunning = false;
        break;
    case KEY_UP:
        cursor.moveRelative(0, -1);
        break;
    case KEY_DOWN:
        cursor.moveRelative(0, 1);
        break;
    case KEY_LEFT:
        cursor.moveRelative(-1, 0);
        break;
    case KEY_RIGHT:
        cursor.moveRelative(1, 0);
        break;
    case KEY_PLACE_STONE: {
        const MoveResult result = game.placeStone(cursor.getX(), cursor.getY());
        setGameStatusMessage(result);
    }	break;
    case KEY_CONFIRM:
        game.confirmPlacement();
        clearGameStatusMessage();
        break;
    case KEY_CANCEL:
        game.cancelPlacement();
        clearGameStatusMessage();
        break;
    case KEY_NEW_GAME:
        createNewGame();
        break;
    case KEY_SAVE:
        saveGame();
        break;
    case KEY_LOAD:
        loadGame();
        break;
    case KEY_FINISH_GAME:
        game.finishGame();
        showGameResult();
        break;
    }
    printGame();
}

void CompleteGame::gameExit() {
    GUIUtils::deinitializeGui();
}

CompleteGame::CompleteGame()
    : game(0),
    cursor(0, 0, 0, 0),
    mainBuffer(GUIUtils::getTerminalWidth(), GUIUtils::getTerminalHeight()),
    boardBuffer(getBoardviewSize(), getBoardviewSize()),
    legendBuffer(LEGEND_WIDTH, LEGEND_HEIGHT) {
    gameIsRunning = true;
}

void CompleteGame::start() {
    gameInit();
    while (gameIsRunning) {
        gameLoop();
    }
    gameExit();
}