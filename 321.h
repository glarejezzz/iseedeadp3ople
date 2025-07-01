#ifndef MARKET_H
#define MARKET_H

#include <iostream>
#include <string>

class EconomicAgent {
protected:
    std::string name;
    double budget;
public:
    EconomicAgent(const std::string& n, double b);
    virtual ~EconomicAgent();

    virtual void makeDecision() = 0;
    virtual void updateBudget(double delta);
    virtual void printInfo() const;

    double getBudget() const;
    std::string getName() const;
};

class Consumer : public EconomicAgent {
private:
    double satisfaction;
public:
    Consumer(const std::string& n, double b);
    void makeDecision() override;
    void printInfo() const override;
};

class Company : public EconomicAgent {
private:
    double productionCost;
    double profitMargin;
public:
    Company(const std::string& n, double b);
    void makeDecision() override;
    void printInfo() const override;
};

class Government : public EconomicAgent {
private:
    double taxRate;
    double subsidyRate;
public:
    Government(const std::string& n, double b);
    void makeDecision() override;
    void applyTax(EconomicAgent* agent);
    void applySubsidy(EconomicAgent* agent);
    void printInfo() const override;
};

class Market {
private:
    EconomicAgent** agents;
    int capacity;
    int size;
    Government* government;

    void expandCapacity();
    void swapAgents(int i, int j);
public:
    Market(int initialCapacity = 10);
    ~Market();

    void addAgent(EconomicAgent* agent);
    void simulateInteractions(int rounds);
    void sortAgentsByBudget();
    void printMarketState() const;
};


