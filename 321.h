#include <string>

class EconomicAgent {
protected:
    double budget;
    std::string name;
public:
    EconomicAgent(double initialBudget, const std::string& agentName);
    virtual ~EconomicAgent() {}
    virtual void makeDecision() = 0;
    virtual void updateBudget(double delta);
    double getBudget() const;
    const std::string& getName() const;
    virtual std::string getType() const = 0;
};

class Consumer : public EconomicAgent {
public:
    Consumer(double initialBudget, const std::string& name);
    void makeDecision() override;
    std::string getType() const override;
};

class Company : public EconomicAgent {
    double productionCost;
public:
    Company(double initialBudget, double cost, const std::string& name);
    void makeDecision() override;
    std::string getType() const override;
    double getProductionCost() const;
};

class Government : public EconomicAgent {
    double taxRate;
    double subsidyRate;
public:
    Government(double initialBudget, double tax, double subsidy, const std::string& name);
    void makeDecision() override;
    void applyTaxesAndSubsidies(EconomicAgent** agents, int count);
    std::string getType() const override;
};

class Market {
    EconomicAgent** agents;
    int agentCount;
    int capacity;
    
    void sortAgentsByBudget();
public:
    Market(int maxCapacity);
    ~Market();
    void addAgent(EconomicAgent* agent);
    void simulateInteractions(int interactions);
    void printTopWealth(int topN);
};
