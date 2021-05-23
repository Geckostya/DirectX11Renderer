#include "CameraMatrix.h"

#include "../Game.h"


CameraMatrix::CameraMatrix(Game* game, float fov) : game(game), fov(fov)
{
    view = DirectX::SimpleMath::Matrix::Identity;
    UpdateProjectionMatrix();
}

void CameraMatrix::SetFow(const float fov)
{
    this->fov = fov;
}

void CameraMatrix::SetView(DirectX::SimpleMath::Matrix view)
{
    this->view = view;
}

DirectX::SimpleMath::Matrix CameraMatrix::Get() const
{
    return view * projection;
}

const DirectX::SimpleMath::Matrix& CameraMatrix::getView() const
{
    return view;
}

const DirectX::SimpleMath::Matrix& CameraMatrix::getProjection() const
{
    return projection;
}

DirectX::SimpleMath::Vector3 CameraMatrix::GetPosition() const
{
    return view.Translation() * -1;
}

DirectX::SimpleMath::Vector3 CameraMatrix::GetForwardVector() const
{
    return view.Invert().Forward();
}

void CameraMatrix::UpdateProjectionMatrix()
{
    projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        fov,
        (float) game->Display->ClientWidth / game->Display->ClientHeight,
        1.0f / 16,
        1 << 14
    );
}
