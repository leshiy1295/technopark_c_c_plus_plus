 // взято из https://ru.wikipedia.org/wiki/%D0%A8%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD%D0%BD%D1%8B%D0%B9_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4_(%D1%88%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F)

/**
 * An abstract class that is common to several games in
 * which players play against the others, but only one is
 * playing at a given time.
 */


class GameObject
{
protected:
    int PlayersCount;

    virtual bool EndOfGame() = 0;
    virtual void InitializeGame() = 0;
    virtual void MakePlay(int player) = 0;
    virtual void PrintWinner() = 0;

public:
    /* A template method: */
    void PlayOneGame(int playersCount)
    {
        PlayersCount = playersCount;
        InitializeGame();

        int j = 0;

        while (!EndOfGame()) {
            MakePlay(j);
            j = (j + 1) % playersCount;
        }

        PrintWinner();
    }
};

class Monopoly : public GameObject
{
protected:
    /* Implementation of necessary concrete methods */
    void InitializeGame() override
    {
        // Initialize money
    }

    void MakePlay(int player) override
    {
        // Process one turn of player
    }

    bool EndOfGame() override
    {
        return true;
    }

    void PrintWinner() override
    {
        // Display who won
    }
};

class Chess : public GameObject
{
protected:
    /* Implementation of necessary concrete methods */
    void InitializeGame() override
    {
        // Put the pieces on the board
    }

    void MakePlay(int player) override
    {
        // Process a turn for the player
    }

    bool EndOfGame() override
    {
        // Return true if in Checkmate or Stalemate has been reached
        return true;
    }

    void PrintWinner() override
    {
        // Display the winning player
    }
};

int main()
{
    GameObject* game = new Monopoly();
   
    game->PlayOneGame(2);

    return 0;
}