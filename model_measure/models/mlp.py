import torch
import torch.nn as nn
import torch.nn.functional as F

class MLP(nn.Module):

  def __init__(self):
    super(MLP, self).__init__()
    self.fc1 = nn.Linear(3072, 384)
    self.fc2 = nn.Linear(384, 384)
    self.fc3 = nn.Linear(384, 384)
    self.fc4 = nn.Linear(384, 384)
    self.fc5 = nn.Linear(384, 384)
    self.fc6 = nn.Linear(384, 10)
    #self.fc7 = nn.Linear(192, 192)
    #self.fc8 = nn.Linear(192, 192)
    #self.fc9 = nn.Linear(192, 192)
    #self.fc10 = nn.Linear(192, 192)
    #self.fc11 = nn.Linear(192, 192)
    #self.fc12 = nn.Linear(192, 10)



  def forward(self, x):
    x = x.view(x.size(0),-1)
    x = F.relu(self.fc1(x))
    x = F.relu(self.fc2(x))
    x = F.relu(self.fc3(x))
    x = F.relu(self.fc4(x))
    x = F.relu(self.fc5(x))
    x = F.relu(self.fc6(x))
    #x = F.relu(self.fc7(x))
    #x = F.relu(self.fc8(x))
    #x = F.relu(self.fc9(x))
    #x = F.relu(self.fc10(x))
    #x = F.relu(self.fc11(x))
    #x = F.relu(self.fc12(x))
    return x

