#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "akinator.h"
#include "database.h"
#include "game_mode.h"
#include "input_output.h"
#include "tree.h"
#include "tree_dump.h"
#include "utils.h"

static char  speak_buffer[MAX_SPEAK_BUFFER_LEN] = {};
static char* cur_speak = speak_buffer;

static const int MAX_ABS_PATH_LEN       = 300;
static const int MAX_RELATIONS_ARR_SIZE = 100;

enum ProgramMode
{
    AKINATOR_MODE,
    COMPARE_MODE,
    DATABASE_MODE,
    INFO_MODE,
    SAVE_QUIT_MODE,
    QUIT_NO_SAVE_MODE,
};

static ProgramMode ChooseProgramMode();
static CodeError   OpenDBBrowser    (TreeNode_t* db_tree);
static CodeError   RunCompareMode   (TreeNode_t* db_tree);
static CodeError   RunInfoMode      (TreeNode_t* db_tree);


static ProgramMode ChooseProgramMode()
{
    printf("\n\n"
           BLU "-----------------------------------------------------------------------------------------" WHT "\n\n"
           MAG
           "     **      **   **  **  ****     **      **     **********   *******    *******   \n"
           "    ****    /**  **  /** /**/**   /**     ****   /////**///   **/////**  /**////**  \n"
           "   **//**   /** **   /** /**//**  /**    **//**      /**     **     //** /**   /**  \n"
           "  **  //**  /****    /** /** //** /**   **  //**     /**    /**      /** /*******   \n"
           " ********** /**/**   /** /**  //**/**  **********    /**    /**      /** /**///**   \n"
           "/**//////** /**//**  /** /**   //**** /**//////**    /**    //**     **  /**  //**  \n"
           "/**     /** /** //** /** /**    //*** /**     /**    /**     //*******   /**   //** \n"
           "//      //  //   //  //  //      ///  //      //     //       ///////    //     //  \n"
           "\n"
           BLU "-----------------------------------------------------------------------------------------" WHT "\n"
           "\n\n");

    PRINT_SPEAK_COLOR_MES(GRN, "Choose one of modes:\n");

    printf(GRN "[A] Akinanor      (wish someone or something and i'll guess it)\n"
               "[C] Compare       (enter two characters and i'll show you their difference)\n"
               "[D] Database      (i'll show you current database tree)\n"
               "[I] Info          (enter character and i'll show you what i know about him)\n"
               "[S] Save and quit (quit with saving tree to database)\n"
               "[Q] Quit          (quit without saving tree to database)\n" WHT "\n");

    int chosen_mode = 0;
    while (true)
    {
        if (chosen_mode != '\n' && chosen_mode != 0)
            ClearBuffer();
        chosen_mode = getchar();

        if (CHECK_USER_ANSWER(chosen_mode, 'a', 'A'))
            return AKINATOR_MODE;

        else if (CHECK_USER_ANSWER(chosen_mode, 'c', 'C'))
            return COMPARE_MODE;

        else if (CHECK_USER_ANSWER(chosen_mode, 'd', 'D'))
            return DATABASE_MODE;

        else if (CHECK_USER_ANSWER(chosen_mode, 'i', 'I'))
            return INFO_MODE;

        else if (CHECK_USER_ANSWER(chosen_mode, 's', 'S'))
            return SAVE_QUIT_MODE;

        else if (CHECK_USER_ANSWER(chosen_mode, 'q', 'Q'))
            return QUIT_NO_SAVE_MODE;

        else
        {
            PRINT_SPEAK_COLOR_MES(RED, "Sorry, i don't know such mode :(\n");
        }
    }

    return AKINATOR_MODE;
}


static CodeError OpenDBBrowser(TreeNode_t* db_tree)
{
    TreeDump(db_tree);
    DumpClose();

    char abs_file_path_buffer[MAX_ABS_PATH_LEN] = {};
    GetFullPathName(DUMP_HTML_FNAME, MAX_ABS_PATH_LEN, abs_file_path_buffer, NULL);

    char command_open_html[MAX_CMD_LEN] = {};
    sprintf(command_open_html, "start %s", abs_file_path_buffer);
    printf("%s", command_open_html);

    if (system(command_open_html) != 0)
        return SYSTEM_CALL_ERR;

    return NO_ERR;
}


CodeError RunAkinator()
{
    CodeError code_err = NO_ERR;

    TreeNode_t* db_tree_root = NULL;
    if ((code_err = ReadDatabase(DATABASE_FILE_NAME, &db_tree_root)) != NO_ERR)
        return code_err;

    if (db_tree_root == NULL)
        return TREE_NOT_INITED_ERR;

    TreeDump(db_tree_root);
    DumpClose();

    ProgramMode cmd_type = QUIT_NO_SAVE_MODE;

    while (true)
    {
        cmd_type = ChooseProgramMode();

        switch (cmd_type)
        {
            case AKINATOR_MODE:
                if ((code_err = RunGameMode(db_tree_root)) != NO_ERR)
                    return code_err;
                break;

            case COMPARE_MODE:
                if ((code_err = RunCompareMode(db_tree_root)) != NO_ERR)
                    return code_err;
                break;

            case DATABASE_MODE:
                if ((code_err = OpenDBBrowser(db_tree_root)) != NO_ERR)
                    return code_err;
                break;

            case INFO_MODE:
                if ((code_err = RunInfoMode(db_tree_root)) != NO_ERR)
                    return code_err;
                break;

            case SAVE_QUIT_MODE:
            {
                code_err = SaveTreeData(DATABASE_FILE_NAME, db_tree_root);

                if (code_err == NO_ERR)
                {
                    PRINT_SPEAK_COLOR_MES(BLU, "Database saved successfully\n");
                    printf(WHT "\n[Press enter to continue]\n");
                    getchar();
                }

                else
                {
                    PRINT_SPEAK_COLOR_MES(RED, "Database wasn't saved because of error\n");
                    printf(WHT "\n[Press enter to continue]\n");
                    getchar();
                }

                return code_err;
            }

            case QUIT_NO_SAVE_MODE:
                return NO_ERR;

            default:
                return UNKNOWN_AKINATOR_MODE_ERR;
                break;
        }
    }

    DumpClose();
    return code_err;
}


#define PRINT_FEATURE_(__first_idx__, __relations_arr_name__)                   \
    if (i != __first_idx__)                                                     \
        cur_speak += sprintf(cur_speak, ", ");                                  \
                                                                                \
    if (__relations_arr_name__[i] == LEFT_SON)                                  \
    {                                                                           \
        cur_speak += sprintf(cur_speak, "NOT %s", cur_node->data);              \
        cur_node = cur_node->left;                                              \
    }                                                                           \
                                                                                \
    else if (__relations_arr_name__[i] == RIGHT_SON)                            \
    {                                                                           \
        cur_speak += sprintf(cur_speak, "%s", cur_node->data);                  \
        cur_node = cur_node->right;                                             \
    }

#define SCAN_CHARACTER_AND_FIND_LEAF_(__tree_name__, __relations_arr_name__, __character_var_name__)                    \
    char __character_var_name__[MAX_STR_LEN] = {};                                                                      \
    scanf("%[^\n]", __character_var_name__);                                                                            \
    ClearBuffer();                                                                                                      \
                                                                                                                        \
    NodesRelation __relations_arr_name__[MAX_RELATIONS_ARR_SIZE] = {};                                                  \
    if (!TreeFindLeaf(__tree_name__, __relations_arr_name__, __character_var_name__, 0))                                \
    {                                                                                                                   \
        PRINT_SPEAK_COLOR_MES(RED, "Sorry, I didn't find such character (%s) in my database\n",                         \
                              __character_var_name__);                                                                  \
        printf(WHT "\n[Press enter to continue]\n");                                                                    \
        getchar();                                                                                                      \
        return NO_ERR;                                                                                                  \
    }

static CodeError RunCompareMode(TreeNode_t* db_tree)
{
    cur_speak = speak_buffer;

    PRINT_SPEAK_COLOR_MES(YEL, "Enter two characters who you want to compare\n");
    SCAN_CHARACTER_AND_FIND_LEAF_(db_tree, relations_arr1, character1);
    SCAN_CHARACTER_AND_FIND_LEAF_(db_tree, relations_arr2, character2);

    printf(YEL);

    cur_speak += sprintf(cur_speak, "Similar features: ");
    int i = 0;
    TreeNode_t* cur_node = db_tree;
    while (relations_arr1[i] == relations_arr2[i] && relations_arr1[i] != NO_RELATION)
    {
        PRINT_FEATURE_(0, relations_arr1);
        ++i;
    }
    cur_speak += sprintf(cur_speak, "\n");

    TreeNode_t* distinctive_node = cur_node;
    int distinctive_i = i;
    cur_speak += sprintf(cur_speak, "Unique features of %s: ", character1);
    while (relations_arr1[i] != NO_RELATION)
    {
        PRINT_FEATURE_(distinctive_i, relations_arr1);
        ++i;
    }
    cur_speak += sprintf(cur_speak, "\n");

    i = distinctive_i;
    cur_node = distinctive_node;
    cur_speak += sprintf(cur_speak, "Unique features of %s: ", character2);
    while (relations_arr2[i] != NO_RELATION)
    {
        PRINT_FEATURE_(distinctive_i, relations_arr2);
        ++i;
    }
    cur_speak += sprintf(cur_speak, "\n");

    PrintVoiceMessage(speak_buffer);
    printf(WHT "\n[Press enter to continue]\n");
    getchar();

    return NO_ERR;
}


static CodeError RunInfoMode(TreeNode_t* db_tree)
{
    cur_speak = speak_buffer;

    PRINT_SPEAK_COLOR_MES(YEL, "Enter the character whose description you want to get\n");
    SCAN_CHARACTER_AND_FIND_LEAF_(db_tree, relations_arr, character)

    printf(YEL);

    cur_speak += sprintf(cur_speak, "Features of %s: ", character);
    TreeNode_t* cur_node = db_tree;
    for (int i = 0; relations_arr[i] != NO_RELATION; ++i)
    {
        PRINT_FEATURE_(0, relations_arr);
    }
    cur_speak += sprintf(cur_speak, "\n");

    PrintVoiceMessage(speak_buffer);
    printf(WHT "\n[Press enter to continue]\n");
    getchar();

    return NO_ERR;
}

#undef PRINT_FEATURE_
#undef SCAN_CHARACTER_AND_FIND_LEAF_
